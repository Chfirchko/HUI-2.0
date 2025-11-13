# HUI - Horoshiy User Interface


## Сборка через CMake


```bash
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
make -j12
```

## Структура папок

- `include/` — заголовочные файлы (.h, .hpp)  
- `include/gui/` — хеддеры для GUI  
- `include/serial/` — хеддеры для serial порта  
- `include/data/` — хеддеры для обработки и сохранения данных  
- `include/common/` — общие хеддеры, если есть  
- `src/` — исходники (.cpp)  
- `src/gui/` — исходники для GUI  
- `src/serial/` — исходники для serial порта  
- `src/data/` — исходники для обработки и сохранения данных  
- `src/main.cpp` — точка входа (если один файл)  
- `resources/` — ресурсы (иконки, шрифты и прочее для GUI)  
- `build/` — сборочные файлы (обычно игнорируется в гите)  
- `CMakeLists.txt` — конфигурация сборки (если используешь CMake)




