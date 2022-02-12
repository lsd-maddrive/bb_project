# Development guide

This is guide how to prepare development environment and use main tools

## Table of contents

- [Table of contents](#table-of-contents)
- [Preparations](#preparations)
- [Optional setup steps](#optional-setup-steps)
- [Some known issues](#some-known-issues)

## Preparations

1. Install make
    - Windows:

        Install [chocolatey](https://chocolatey.org/install) and install `make` with command:

    ```powershell
    choco install make
    ```

    - Linux:

    ```bash
    sudo apt-get install build-essential
    ```

1. Install python 3.8
    - Windows

        Install with [official executable](https://www.python.org/downloads/)

    - Linux

    ```bash
    sudo apt install python3.8-dev
    ```

1. Install poetry

   - Windows

        Use [official instructions](https://python-poetry.org/docs/#windows-powershell-install-instructions) or use `powershell` command:

    ```powershell
    (Invoke-WebRequest -Uri https://raw.githubusercontent.com/python-poetry/poetry/master/get-poetry.py -UseBasicParsing).Content | python -
    ```

   - Linux

    ```bash
    make poetry-download
    ```

2. Initialize poetry and install `pre-commit` hooks:

```bash
make install
```

[Table of contents](#table-of-contents)

## Optional setup steps

1. Install VSCode Extensions
   - [EditorConfig for VS Code](https://marketplace.visualstudio.com/items?itemName=EditorConfig.EditorConfig)
      Open control panel (Ctrl+P) and enter `ext install EditorConfig.EditorConfig`

1. To initialize generation of Table of Contents from notebook headers we use `nbextension`:

    - `nbextention-toc-install`

    > It is required version nbconvert~=5.6.1 (checked for date 2021-12-29)

    - To export notebook with ToC use next command:

      ```bash
      poetry run jupyter nbconvert --template toc2 --to html_toc --output-dir ./exports <путь до файла>
      ```

      > For example, `poetry run jupyter nbconvert --template toc2 --to html_toc --output-dir ./exports notebooks/example.ipynb`

[Table of contents](#table-of-contents)

## Some known issues

- If you have issues with python version like:

    ```bash
    The currently activated Python version 3.9.7 is not supported by the project (~3.8.0)
    ...
    NoCompatiblePythonVersionFound
    ...
    ```

    Check version of your `python3` binary and make sure you have python3.8 installed.

[Table of contents](#table-of-contents)


Некоторые [правила оформления](https://github.com/serykhelena/bb-8_project/blob/develop/controller_bb_8_driver/docs/dev_rules.md)


## Как изменить код на винде, но прошить через малину

1. После того, как проект собран - запустить батник `move_bin.bat` - он переместит файл `ch.bin` в расшаренную папку (убедитесь, что она есть и креды правильные, иначе доступа не будет).

2. В малине запустить скрипт `pi_load_stm.sh`

```bash
./pi_load_stm.sh
```
[Table of contents](#table-of-contents)
