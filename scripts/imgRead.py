def get_bytes_from_file(filename):
    return open(filename, "rb").read()

if __name__ == "__main__":
    array = get_bytes_from_file("porsche.png")

    width = 0

    print("#pragma once\n")
    print("const uint8_t IMAGE_DATA[] = {")
    for x in array:
        print(hex(x),end=",")
        width = (width + 1) % 20
        if width == 0:
            print("\n",end="")
    print("\n};")
    print("const uint32_t IMAGE_SIZE = sizeof(IMAGE_DATA);")
