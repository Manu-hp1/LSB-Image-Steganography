🖼️ LSB Image Steganography (C)
📌 Overview

This project implements Least Significant Bit (LSB) Image Steganography using C programming.
It securely hides secret data inside BMP images and allows accurate extraction without causing any noticeable visual distortion.

The project focuses on bit-level manipulation, binary file processing, and memory management, making it a strong demonstration of low-level programming skills.

🎯 Objectives

Hide secret text data inside an image using the LSB technique

Extract the hidden data reliably from the encoded image

Understand binary file formats and bitwise operations

Preserve original image quality after data embedding

🛠️ Technologies & Concepts

Language: C

Core Concepts:

Bitwise Operations

File Handling (Binary I/O)

Memory Management

Image Format: BMP (24-bit)

⚙️ How It Works

Each pixel in a BMP image contains RGB byte values.

The least significant bit of each byte is modified to store one bit of secret data.

Since only LSBs are altered, the image appears unchanged to the human eye.

During decoding, these LSBs are read back to reconstruct the original message.

🔍 This ensures data secrecy while maintaining visual integrity.

✨ Key Features

Encode and decode secret data using LSB technique

Supports BMP image format

Minimal impact on image quality

Command-line based implementation

Robust input validation and error handling

🚧 Challenges Faced

Managing binary file offsets accurately

Ensuring data integrity while modifying only LSBs

Preventing image corruption during encoding

Efficient memory handling during large file operations

▶️ Compilation & Execution
🔧 Compile
gcc *.c -o steganography

🔐 Encode (Hide Data)
./steganography -e source.bmp secret.txt encoded.bmp

🔓 Decode (Extract Data)
./steganography -d encoded.bmp output.txt

📂 Project Structure
LSB-Steganography/
│── encode.c
│── decode.c
│── common.c
│── types.h
│── README.md

📈 Learning Outcomes

Strong understanding of LSB-based steganography

Hands-on experience with bit manipulation

Improved proficiency in binary file I/O

Better grasp of secure data hiding techniques

🚀 Future Enhancements

Support for additional image formats (PNG, JPEG)

Encrypt data before embedding 🔐

Password-protected decoding

Improved data capacity and robustness

👤 Author

Manu H P
Embedded Systems | C | Linux
PES College of Engineering, Mandya
