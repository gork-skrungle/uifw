mkdir -p "./_compiled/"
mkdir -p "./_compiled/SPIRV/"

# ------------------ This script requires the SDL_shadercross CLI ------------------

#
#   The latest DirectX Shader Compiler must be downloaded and available in the
#   current shell configuration
#

for filename in *.vert.hlsl; do
    if [ -f "$filename" ]; then
        shadercross "$filename" -o "_compiled/SPIRV/${filename/.hlsl/.spv}"
    fi
done

for filename in *.frag.hlsl; do
    if [ -f "$filename" ]; then
        shadercross "$filename" -o "_compiled/SPIRV/${filename/.hlsl/.spv}"
    fi
done
