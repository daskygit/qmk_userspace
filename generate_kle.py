#github.com/guanzd88/qmk_to_kle_converter

import argparse
import os
import json
import pyclip

def find_info_json_folders(root_folder):
    info_json_folders = []
    for root, dirs, files in os.walk(root_folder):
        if 'keyboard.json' in files:
            relative_path = os.path.relpath(root, root_folder)
            info_json_folders.append(relative_path)
    return info_json_folders

def generate_kle_layout(info_json_path):
    try:
        with open(info_json_path, 'r') as f:
            jsonObj = json.load(f)
    except json.JSONDecodeError as e:
        return None
    if 'layouts' not in jsonObj:
        return None
    dict_layouts_obj = jsonObj['layouts']
    dict_keyboard_layout = dict_layouts_obj[list(dict_layouts_obj)[0]]['layout']

    layout_dict = {}
    for item in dict_keyboard_layout:
        y_value = item['y']
        if y_value not in layout_dict:
            layout_dict[y_value] = []
        layout_dict[y_value].append(item)

    layout_dict = dict(sorted(layout_dict.items()))

    for y_value, key_list in layout_dict.items():
        prev_key = None
        for item in key_list:
            if prev_key is not None:
                key_diff = item['x'] - prev_key
                item['key_diff'] = key_diff - 1
            prev_key = item['x']

    result_dict = {list(layout_dict)[0]: list(layout_dict)[0]}

    for i in range(1, len(layout_dict)):
        diff = list(layout_dict)[i] - list(layout_dict)[i - 1]
        result_dict[list(layout_dict)[i]] = diff - 1

    output = ''
    for key, value in layout_dict.items():
        line = []
        for i in range(len(value)):
            item_layout_dict = value[i]
            if i == 0:
                template = {}
                template["y"] = result_dict[key]
                if 'key_diff' in item_layout_dict:
                    template["x"] = item_layout_dict['key_diff']
                else:
                    template["x"] = item_layout_dict['x']
                line.append(template)
                try:
                    line.append(f'''{item_layout_dict['matrix']}''')
                except KeyError:
                    pass
            else:
                template = {}
                if 'key_diff' in item_layout_dict:
                    template["x"] = item_layout_dict['key_diff']
                else:
                    template["x"] = item_layout_dict['x']
                line.append(template)
                try:
                    line.append(f'''{item_layout_dict['matrix']}''')
                except KeyError:
                    pass
        output += str(line).replace("'x'",'x').replace("'y'",'y').replace("\'",'\"')+',\n'
    pyclip.copy(str(output))
    return output

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Generate KLE JSON from a QMK keyboard.json file or keyboard folder.'
    )
    parser.add_argument(
        'keyboard_folder',
        help='Path to the keyboard folder or root folder containing keyboard layouts.',
    )
    parser.add_argument(
        '--output',
        default='.',
        help='Output directory or file path for generated KLE JSON. Use a .json file name to write a single file.',
    )
    args = parser.parse_args()

    keyboards_folder = os.path.abspath(args.keyboard_folder)
    output_path = os.path.abspath(args.output)

    if os.path.isfile(keyboards_folder) and os.path.basename(keyboards_folder) == 'keyboard.json':
        keyboards_folder = os.path.dirname(keyboards_folder)

    info_json_folders = find_info_json_folders(keyboards_folder)

    output_is_file = os.path.splitext(output_path)[1].lower() == '.json' or os.path.isfile(output_path)
    if output_is_file and len(info_json_folders) != 1:
        raise SystemExit('Error: --output <file.json> may only be used when generating a single layout.')

    if output_is_file:
        output_file = output_path
        output_dir = os.path.dirname(output_file)
        os.makedirs(output_dir, exist_ok=True)
    else:
        kle_folder = output_path
        os.makedirs(kle_folder, exist_ok=True)

    for folder in info_json_folders:
        info_json_path = os.path.join(keyboards_folder, folder, 'keyboard.json')
        kle_json = generate_kle_layout(info_json_path)
        if kle_json is not None:
            if output_is_file:
                with open(output_file, 'w') as f:
                    f.write(kle_json)
            else:
                kle_path = os.path.join(kle_folder, folder)
                os.makedirs(kle_path, exist_ok=True)
                with open(os.path.join(kle_path, 'kle.json'), 'w') as f:
                    f.write(kle_json)
