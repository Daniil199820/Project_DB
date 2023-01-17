# Project_DB
Проект представляет собой SQL - реляционную базу данных.
SQL-запрос обрабатывается с помощью следующих модулей:
·Splitter
·Parcer
·DB_request

Класс "Splitter" разделяет запрос на слова, разделенные табуляциями, пробелами и концами строки. Выходными данными работы основной функции класса является очередь std::string.
Пример работы: 
Исходная строка
"CREATE TABLE table_name(CUSTOM_INT name_column)" ->
Выходная очередь
"CREATE" "TABLE" "table_name" "(" "CUSTOM_INT" "name_column" ")"

Класс "Parcer" проводит токенизацию запроса на следующие токены:
·COMMAND
·Value
·String_value
·ARG_START
·ARG_FINISH
·COMA
Выходными данными работы основной функции класса является хэш-таблица, состоящая из ключа std::string и значения - соответствующего токена.
Пример работы:
Исходная строка
"CREATE TABLE table_name(CUSTOM_INT name_column)" ->
Выходная строка
"CREATE" - "Command"
"TABLE" - "Command"
"table_name" - "Value"
"(" - "ARG_START"
"CUSTOM_INT" - "Type"
"table_name" - "Value"
")" - "ARG_FINISH"
 
Модуль DB_request основан на паттерне машине состояний. В зависимости от запроса контекст переходит в то или иное состояние. Состояния выражены с помощью классов 