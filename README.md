# Учебная графическая библиотека с использованием DirectX 9

## Описание

Реализованные функции:

1. [bar](https://www.cs.colorado.edu/~main/bgi/doc/bar.html)
2. [fillellipse](https://www.cs.colorado.edu/~main/bgi/doc/fillellipse.html) Обводка пока не поддерживается, нужно сначала вызвать `setlinestyle(NULL_LINE, 0, 0)`
3. [putpixel](https://www.cs.colorado.edu/~main/bgi/doc/putpixel.html)
4. [rectangle](https://www.cs.colorado.edu/~main/bgi/doc/rectangle.html)
5. [setfillstyle](https://www.cs.colorado.edu/~main/bgi/doc/setfillstyle.html) / [getfillsettings](https://www.cs.colorado.edu/~main/bgi/doc/getfillsettings.html)
6. [setfillpattern](https://www.cs.colorado.edu/~main/bgi/doc/setfillpattern.html) / [getfillpattern](https://www.cs.colorado.edu/~main/bgi/doc/getfillpattern.html)
7. [setlinestyle](https://www.cs.colorado.edu/~main/bgi/doc/setlinestyle.html) / [getlinesettings](https://www.cs.colorado.edu/~main/bgi/doc/getlinesettings.html)
8. [setcolor](https://www.cs.colorado.edu/~main/bgi/doc/setcolor.html) / [getcolor](https://www.cs.colorado.edu/~main/bgi/doc/getcolor.html) , [setbgcolor](https://www.cs.colorado.edu/~main/bgi/doc/setbkcolor.html) / [getbgcolor](https://www.cs.colorado.edu/~main/bgi/doc/getbkcolor.html)

Дополнительные возможности:
1. Возможность создания нескольких окон: `directgraph_create_window`. Функции с суффиксом `w`, для рисования в указанном окне.
2. Полупрозрачные цвета, для всего, кроме `putpixel`. Для указания прозрачности используется старший байт. Например, `0xFF0000FF` — полностью прозрачный красный, `0x7FFF0000` — наполовину прозрачный синий.

Особенности:

1. Используется буфер глубины для минимизации изменений состояния (state changes) и избыточной перерисовки (overdraw)
2. Используются шейдеры. Если они не поддерживаются видеокартой, то обычные треугольники.
3. Поддержка экранов с нестандартным DPI (DPI awareness)
4. Нить-безопасность
5. Последовательные вызовы `putpixel` с любыми координатами накапливаются и потом обрабатываются все сразу.

## Совместимость

Библиотека работает на системах Windows, начиная Windows XP SP2, x86 и x64. Так как используется C API, то подключить её может практически любой современный компилятор.

Саму же библиотеку можно скомпилировать с помощью Visual Studio 2010, 2012, 2015 и MinGW-w64.