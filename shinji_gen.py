import json
import os
import subprocess

req_env = [
    'SHINJI_HOME',
    'CMAKE_CURRENT_SOURCE_DIR',
    'GLSLC'
]

for env_var in req_env:
    if env_var not in os.environ:
        raise Exception("Missing required env variable: %s" % env_var)


def get_shader_id(shader):
    return ''.join(['{:02x}'.format(ord(x)) for x in shader])


def get_shader_var_name(shader):
    return "_%s" % get_shader_id(shader)


def fix_path(path):
    if os.path.isabs(path):
        return path
    else:
        return os.path.join(os.environ['CMAKE_CURRENT_SOURCE_DIR'], path)


def create_c_var_from_str(var_name, byte_arr):
    var_val = ','.join([str(x) for x in byte_arr])
    return "char const %s[] = { %s };" % (var_name, var_val)


def create_c_var_from_plain_shader(shader):
    with open(fix_path(shader), 'rt') as shader_f:
        shader_content = shader_f.read()
        return create_c_var_from_str(get_shader_var_name(shader), str.encode(shader_content))


def create_c_var_from_spirv_shader(shader, input_shader, glslc_options):
    cmd = [os.environ['GLSLC'], fix_path(input_shader), "-o", "-"]
    if glslc_options:
        cmd = cmd + [glslc_options]

    glslc_proc = subprocess.run(cmd, capture_output=True)
    if glslc_proc.returncode != 0:
        raise Exception("glslc failed: %s" % ' '.join(cmd))
    return create_c_var_from_str(get_shader_var_name(shader), glslc_proc.stdout)


def load_config(cfg_path):
    with open(cfg_path, 'rt') as cfg_f:
        cfg = json.loads(cfg_f.read())

        with open(fix_path(cfg['generated_file']), 'wt') as bundle_f:
            if "bundle" in cfg:
                bundle_f.writelines('\n'.join([
                    '#pragma once',
                    '',
                    '#include <string>',
                    '#include <unordered_map>',
                    '',
                    'namespace shinji::bundle',
                    '{', ''
                ]))

                for shader in cfg['bundle']['shaders']:
                    if type(shader) == str:
                        bundle_f.write('\n'.join([
                            create_c_var_from_plain_shader(shader),
                            ''
                        ]))
                    else:
                        if shader['type'] == 'glslc':
                            bundle_f.write('\n'.join([
                                create_c_var_from_spirv_shader(
                                    shader['name'],
                                    shader['input'],
                                    shader['glslc_options'] if 'glslc_options' in shader else ''
                                ),
                                ''
                            ]))
                        else:
                            print("Unsupported shader bundling type: %s" % shader['type'])

                bundle_f.writelines('\n'.join([
                    "inline std::unordered_map<std::string, std::pair<char const*, size_t>> const s_src_by_shader{", '',
                ]))

                for shader in cfg['bundle']['shaders']:
                    shader = shader if type(shader) == str else shader['name']
                    shader_var_name = get_shader_var_name(shader)
                    bundle_f.writelines('\n'.join([
                        '{ "%s", { %s, sizeof(%s) / sizeof(char) }},' % (shader, shader_var_name, shader_var_name),
                        ''
                    ]))

                bundle_f.writelines('\n'.join([
                    '};', '',
                ]))

                bundle_f.writelines('\n'.join([
                    'inline std::pair<char const*, size_t> load_shader(char const* shader)',
                    '{',
                    '    return shinji::bundle::s_src_by_shader.at(shader);',
                    '}', ''
                ]))

                bundle_f.writelines('\n'.join([
                    '}', ''
                ]))

                with open(os.path.join(os.environ['SHINJI_HOME'], 'src', 'shinji_loader.inl'), 'rt') as loader_f:
                    bundle_f.write(loader_f.read())


if __name__ == "__main__":
    import sys

    if len(sys.argv) != 2:
        raise Exception("Invalid syntax: %s <config_file>" % sys.argv[0])

    load_config(sys.argv[1])
