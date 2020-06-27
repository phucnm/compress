# compress
A simple compressor implementing [LZW](https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv%E2%80%93Welch) for educational purpose. This program is compliant to `compress` tool on Linux which means that we can use `compress` to decompress files generated from this program.

## Usage
- Build code with `make`
- Compress a file by `./uvcompress < inputfile > inputfile.z`
- Optional: Decompress the above ouput with `compress` by `compresss -d < inputfile.z > inputfile`

