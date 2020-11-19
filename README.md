# readfile_csv
Контроль звонков по выписке мобильного оператора Теле2.
Для анализа необходимо взять таблицу «Звонки, интернет, SMS- и MMS-сообщения» и конвертировать её в формат csv.
Для конвертации файла я пользовался сервисом https://www.pdf2go.com/ru/pdf-to-excel.
Поля таблицы:
"Дата и время", "Тарифный план", "Тип соединения", "Номер", "Длительность в секундах", "Длительность в минутах", "Сумма к оплате".
Программа попросит ввести путь и имя файла для анализа.
Программа выведет два отсортированных массива: Номер телефона + общая продолжительнось разговоров, номер телефона + число звонков.
Далее будет запрошен номер телефона для которого будет выведена табличка звонков по датам с указанием времени звонка, продолжительности и типа (входящий/исходящий).
Последняя табличка группирует звонки по дням недели с указанием общего времени, числа звонков, времени вызова и типа звонка.

Писал для себя. Код ужасен, но надо было проанализировать одну таблицу. Если нужен такой сервис, могу сделать на сайте, пишите.