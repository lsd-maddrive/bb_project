# BB-ka project

<div align="center">

[![PythonSupported](https://img.shields.io/badge/python-3.8-brightgreen.svg)](https://python3statement.org/#sections50-why)
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)
[![Pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://pre-commit.com/)
[![Bandit](https://img.shields.io/badge/security-bandit-informational.svg)](https://github.com/PyCQA/bandit)

</div>

Проект под командной разработкой, направленный на создание прикольной зверюхи a-ля BB-8 Звёздных войн, но с блэкджеком и силовыми драйверами.

<p align=center>
<img src="docs/images/bb1.jpg" width=300/>
</p>

## Системные требования

- Версия Python: 3.8
- Операционная система: Ubuntu 20.04

## Настройка проекта

1. Установка 'make'

    * Windows - Установите [chocolatey](https://chocolatey.org/install) и установите 'make' командой:
    ```
    choco install make
    ```
    * Linux
    ```
    sudo apt-get install build-essential
    ```

2. Установите Python 3.8
    * Windows - Установите с [официального сайта](https://www.python.org/downloads/)
    * Linux
    ```
    sudo apt install python3.8-dev
    ```
3. Установите `poetry`
   * Windows - Используйте [официальные инструкции](https://python-poetry.org/docs/#windows-powershell-install-instructions) или команду powershell:
   ```
   (Invoke-WebRequest -Uri https://raw.githubusercontent.com/python-poetry/poetry/master/get-poetry.py -UseBasicParsing).Content | python -
   ```
   * Linux
    ```
    make poetry-download
    ```
4. Установите форматтеры и другие инструменты для разработки
    ```
    make tools-install
    ```
5. При работе с Jupyter-ноутбуками используйте виртуальное окружение, созданное `poetry` - `.venv`


## Полезные ссылки

* [F767ZI Reference manual (RM)](http://www.st.com/content/ccc/resource/technical/document/reference_manual/group0/96/8b/0d/ec/16/22/43/71/DM00224583/files/DM00224583.pdf/jcr:content/translations/en.DM00224583.pdf)
* [F767ZI Datasheet (DS)](http://www.st.com/content/ccc/resource/technical/document/datasheet/group3/c5/37/9c/1d/a6/09/4e/1a/DM00273119/files/DM00273119.pdf/jcr:content/translations/en.DM00273119.pdf)
* [F767ZI Nucleo pinout](https://os.mbed.com/platforms/ST-Nucleo-F767ZI/)
---
* [F767ZI Alternate functions](http://www.st.com/content/ccc/resource/technical/document/datasheet/group3/c5/37/9c/1d/a6/09/4e/1a/DM00273119/files/DM00273119.pdf/jcr:content/translations/en.DM00273119.pdf#page=89)
* [F767ZI Pins assignment (+analogs)](http://www.st.com/content/ccc/resource/technical/document/datasheet/group3/c5/37/9c/1d/a6/09/4e/1a/DM00273119/files/DM00273119.pdf/jcr:content/translations/en.DM00273119.pdf#page=65)
* [F767ZI Timer triggers](http://www.st.com/content/ccc/resource/technical/document/reference_manual/group0/96/8b/0d/ec/16/22/43/71/DM00224583/files/DM00224583.pdf/jcr:content/translations/en.DM00224583.pdf#page=452)
