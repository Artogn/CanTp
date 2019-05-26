import argparse
import os
from json import load
from pycantp import CodeGen


def main():
    parser = argparse.ArgumentParser(prog='pycantp',
                                     description='post-build configuration generator for CanTp module')

    parser.add_argument('configuration', help='full path to JSON configuration')
    parser.add_argument('-source', help='output source file path')
    parser.add_argument('-header', help='output header file path')

    args = parser.parse_args()

    with open(args.configuration, 'r') as fp:
        data = load(fp)

    with open(os.path.join(os.path.dirname(__file__), 'pycantp', 'template', 'config.schema.json'), 'r') as fp:
        schema = load(fp)

    code_generator = CodeGen(schema, data)

    if args.source:
        with open(args.source, 'w') as fp:
            fp.write(code_generator.source)

    if args.header:
        with open(args.header, 'w') as fp:
            fp.write(code_generator.header)


if __name__ == '__main__':
    main()
