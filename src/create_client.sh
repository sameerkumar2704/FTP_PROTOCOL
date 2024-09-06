
echo "Starting build process..."


gcc client.c -o client.out -L. ../headers/*.a -lzip -pthread

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Build successful. Output file: client.out"
else
    echo "Build failed."
fi
