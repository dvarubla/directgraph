# Учебная графическая библиотека с использованием DirectX 9

## Описание

Реализованные функции:

1. [bar](https://www.cs.colorado.edu/~main/bgi/doc/bar.html)
2. [fillellipse](https://www.cs.colorado.edu/~main/bgi/doc/fillellipse.html) / [drawellipse](https://www.cs.colorado.edu/~main/bgi/doc/drawellipse.html) / [circle](https://www.cs.colorado.edu/~main/bgi/doc/circle.html) Обводка c узором не поддерживается
3. [putpixel](https://www.cs.colorado.edu/~main/bgi/doc/putpixel.html)
4. [rectangle](https://www.cs.colorado.edu/~main/bgi/doc/rectangle.html)
5. [fillpoly](https://www.cs.colorado.edu/~main/bgi/doc/fillpoly.html) / [drawpoly](https://www.cs.colorado.edu/~main/bgi/doc/drawpoly.html)
6. [bar3d](https://www.cs.colorado.edu/~main/bgi/doc/bar3d.html)
7. [line](https://www.cs.colorado.edu/~main/bgi/doc/line.html) / [lineto](https://www.cs.colorado.edu/~main/bgi/doc/lineto.html) / [linerel](https://www.cs.colorado.edu/~main/bgi/doc/linerel.html)
8. [moveto](https://www.cs.colorado.edu/~main/bgi/doc/moveto.html) / [moverel](https://www.cs.colorado.edu/~main/bgi/doc/moverel.html), [getx](https://www.cs.colorado.edu/~main/bgi/doc/getx.html) / [gety](https://www.cs.colorado.edu/~main/bgi/doc/gety.html)
9. [getmaxx](https://www.cs.colorado.edu/~main/bgi/doc/getmaxx.html) / [getmaxy](https://www.cs.colorado.edu/~main/bgi/doc/getmaxy.html)
10. [setfillstyle](https://www.cs.colorado.edu/~main/bgi/doc/setfillstyle.html) / [getfillsettings](https://www.cs.colorado.edu/~main/bgi/doc/getfillsettings.html)
11. [setfillpattern](https://www.cs.colorado.edu/~main/bgi/doc/setfillpattern.html) / [getfillpattern](https://www.cs.colorado.edu/~main/bgi/doc/getfillpattern.html)
12. [setlinestyle](https://www.cs.colorado.edu/~main/bgi/doc/setlinestyle.html) / [getlinesettings](https://www.cs.colorado.edu/~main/bgi/doc/getlinesettings.html)
13. [setcolor](https://www.cs.colorado.edu/~main/bgi/doc/setcolor.html) / [getcolor](https://www.cs.colorado.edu/~main/bgi/doc/getcolor.html) , [setbgcolor](https://www.cs.colorado.edu/~main/bgi/doc/setbkcolor.html) / [getbgcolor](https://www.cs.colorado.edu/~main/bgi/doc/getbkcolor.html)

Дополнительные возможности:
1. Возможность создания нескольких окон: `directgraph_create_window`. Функции с суффиксом `w`, для рисования в указанном окне.
2. Возможность передавать дополнительный цвет рисования, аналогично `setcolor`, для функций с суффиксом `c`.
3. Работа с палитрой: `setpalette`, `clearpalette`, `initpalette`. Также есть макроопределения `MAXCOLOR16`, `MAXCOLOR256`, которые можно определить до подключения заголовка библиотеки.
4. Полупрозрачные цвета, для всего, кроме `putpixel`. Для указания прозрачности используется старший байт. Например, `0xFF0000FF` — полностью прозрачный красный, `0x7FFF0000` — наполовину прозрачный синий.

Особенности:

1. Используется буфер глубины для минимизации изменений состояния (state changes) и избыточной перерисовки (overdraw)
2. Используются шейдеры. Если они не поддерживаются видеокартой, то обычные треугольники.
3. Поддержка экранов с нестандартным DPI (DPI awareness)
4. Нить-безопасность
5. Последовательные вызовы `putpixel` с любыми координатами накапливаются и потом обрабатываются все сразу.

## Совместимость

Библиотека работает на системах Windows, начиная Windows XP SP2, x86 и x64. Так как используется C API, то подключить её может практически любой современный компилятор.

Саму же библиотеку можно скомпилировать с помощью Visual Studio 2010, 2012, 2015 и MinGW-w64.