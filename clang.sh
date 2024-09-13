#!/bin/bash

find . -iname *.h -o -iname *.m -o -iname *.mm | grep -v -e Pods -e build | xargs clang-format -i -n --Werror