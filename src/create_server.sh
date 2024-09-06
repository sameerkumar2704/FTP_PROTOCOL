
echo "Starting build server..."

# Compile the C source file
gcc server.c -o server.out -L. ../headers/*.a -lzip -pthread

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Build successful. Output file: server.out"
else
    echo "Build failed."
fi
