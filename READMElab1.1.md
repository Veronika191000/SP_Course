Необходимо написать программу, которая бы генерировала ошибку в ходе выполнения системного вызова и выдавала системное описание данной ошибки. Вторым режимом работы данной программы должен быть вывод информации о состоянии системы. Режим запуска программы определяется ключом, передаваемым в командной строке (-e – печать ошибки, -s – печать информации о системе).

Key -e generate error :<br>
Fail generation. Code : 0x8<br>
Error description message: Недостаточно памяти для обработки команды<br>

Key -s generate information about system: <br>
 Hardware information: <br>
  OEM ID: 0 <br>
  Number of processors: 4 <br>
  Page size: 4096 <br>
  Processor type: 586 <br>
  Minimum application address: 10000 <br>
  Maximum application address: 7ffeffff <br>
  Active processor mask: 15 <br>




