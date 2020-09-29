#emcc -o chip-8.html ../src/main.cpp -O3 -s WASM=1 --shell-file ../resources/html_templates/gui.html -s NO_EXIT_RUNTIME=1  -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" --bind
emcc -o chip-8.html ../src/main.cpp -O3 \
    -s WASM=1 \
    --shell-file ../resources/html/gui.html \
    -s NO_EXIT_RUNTIME=1  \
    -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" \
    --bind \
    --embed-file ./ROMS/pong.rom
    
python3 -m http.server