# 3D-OpenGL-Project

## Описание
Это учебные примеры построения 3D моделей с помощью OpenGL. Основан на лабораторных работах НИУ МИЭТ.
В обоих проектах тестируются вершинный и фрагментный шейдеры.
Lab1 - содержит модели освещения Фонга
Lab2 - содержит шейдеры для карты высот

## Установка
Проекты выполнены в среде Microsoft Visual Studio 2019, ОС Windows 10 х64.  
Для корректной работы после открытия проекты необходимо указать пути к библиотекам glut, glew и SOIL2.  
Для этого:  
1) Перейдите: "Проект -> свойства -> С/С++ -> Дополнительные каталоги включаемых библиотек"  
Укажите здесь пути к папкам:
glut  
glew-2.1\glew-2.1.0\include\GL  
SOIL2-master\src\SOIL2  

2) Перейдите: "Проект -> свойства -> Компоновщик -> Общие -> Дополнительные каталоги библиотек  
Укажите здесь пути к папкам:   
glut  
glew-2.1\glew-2.1.0\lib\Release\Win32  (или Ваша версия ОС)  
SOIL2-master\lib\windows (или Ваша версия ОС)  
  
Можно так же скачать более актуальные версии библиотек из других источников, скомпилировать и добавить их в пути проекта.  
  
## Примеры работы:  
Lab1:  
![image](https://user-images.githubusercontent.com/47564437/109459198-676bab00-7a6f-11eb-97a4-c5a93c4c8e53.png)

Lab2:  
![image](https://user-images.githubusercontent.com/47564437/109459391-bdd8e980-7a6f-11eb-8107-0b61f9708e4a.png)
