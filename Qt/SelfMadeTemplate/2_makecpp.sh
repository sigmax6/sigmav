#!/bin/bash

File=hello
uic -o {$File}.h {$File}.ui
uic -o {$File}.cpp -impl {$File}.h {$File}.ui
moc -o {$File}.h -o moc_{$File}.cpp
