mkdir -p "./_generated/"

for filename in *.ttf; do
    if [ -f "$filename" ]; then
        ../../bin/msdf-atlas-gen/linux/msdf-atlas-gen -font "$filename" -allglyphs -format png -imageout "./_generated/${filename/.ttf/.png}" -json "./_generated/${filename/.ttf/.json}"
    fi
done
