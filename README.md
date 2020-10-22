# BB-8 project

Проект под командной разработкой, направленный на создание прикольной зверюхи из Звёздных войн   

Некоторые [правила оформления](https://github.com/serykhelena/bb-8_project/blob/develop/controller_bb_8_driver/docs/dev_rules.md)

Полезные ссылки:
* [F767ZI Reference manual (RM)](http://www.st.com/content/ccc/resource/technical/document/reference_manual/group0/96/8b/0d/ec/16/22/43/71/DM00224583/files/DM00224583.pdf/jcr:content/translations/en.DM00224583.pdf)
* [F767ZI Datasheet (DS)](http://www.st.com/content/ccc/resource/technical/document/datasheet/group3/c5/37/9c/1d/a6/09/4e/1a/DM00273119/files/DM00273119.pdf/jcr:content/translations/en.DM00273119.pdf)
* [F767ZI Nucleo pinout](https://os.mbed.com/platforms/ST-Nucleo-F767ZI/)
---
* [F767ZI Alternate functions](http://www.st.com/content/ccc/resource/technical/document/datasheet/group3/c5/37/9c/1d/a6/09/4e/1a/DM00273119/files/DM00273119.pdf/jcr:content/translations/en.DM00273119.pdf#page=89)
* [F767ZI Pins assignment (+analogs)](http://www.st.com/content/ccc/resource/technical/document/datasheet/group3/c5/37/9c/1d/a6/09/4e/1a/DM00273119/files/DM00273119.pdf/jcr:content/translations/en.DM00273119.pdf#page=65)
* [F767ZI Timer triggers](http://www.st.com/content/ccc/resource/technical/document/reference_manual/group0/96/8b/0d/ec/16/22/43/71/DM00224583/files/DM00224583.pdf/jcr:content/translations/en.DM00224583.pdf#page=452)

## Как изменить код на винде, но прошить через малину 

1. После того, как проект собран - запустить батник `move_bin.bat` - он переместит файл `ch.bin` в расшаренную папку (убедитесь, что она есть и креды правильные, иначе доступа не будет).

2. В малине запустить скрипт `pi_load_stm.sh`

```bash
./pi_load_stm.sh 
``` 