const stb = @import("lib.zig").stb;

pub const img = @This();

width : u32,
height : u32,
pitch : u32,
raw : []u8,

pub fn create(data: []const u8) !img {
    var i : img = undefined;

    var width: c_int = undefined;
    var height: c_int = undefined;

    if (stb.stbi_info_from_memory(
            data.ptr,
            @intCast(c_int, data.len),
            &width,
            &height,
            null
        ) == 0
    ) {
        return error.NotPngFile;
    }

    if (i.width <= 0 or i.height <= 0) return error.NoPixels;
    i.width = @intCast(u32, width);
    i.height = @intCast(u32, height);

    if (stb.stbi_is_16_bit_from_memory(
            data.ptr,
            @intCast(c_int, data.len)
        ) != 0
    ) {
        return error.InvalidFormat;
    }

    const bits_per_channel = 8;
    const channel_count = 4;

    const image_data = stb.stbi_load_from_memory(
        data.ptr,
        @intCast(c_int, data.len),
        &width,
        &height,
        null,
        channel_count
    );

    if (image_data == null) return error.NoMem;

    i.pitch = i.width * bits_per_channel * channel_count / 8;
    i.raw = image_data[0 .. i.height * i.pitch];

    return i;
}

pub fn destroy(i: *const img) void {
    stb.stbi_image_free(i.raw.ptr);
}

//************************************************/
//                    TESTING                    */
//************************************************/

const std = @import("std");

test "stb_invalid_data" {
    const test_img = "iehdvjviasrtnrst";

    const i = img.create(test_img);
    try std.testing.expectError(error.NotPngFile, i);
}

test "stb_valid_data" {
    const test_img = @embedFile("test/car.png");

    const i = try img.create(test_img);
    defer i.destroy();
}

