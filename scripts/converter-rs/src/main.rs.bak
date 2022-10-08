use image::{Rgb, RgbImage};

struct ScreenInfo {
    name: String,
    code: u16,
    palette: Vec<Rgb<u8>>
}

enum ScreenType {
    S565c,
    S16
}

impl ScreenType {
    fn value(&self) -> ScreenInfo {
        match self {
            ScreenType::S565c => ScreenInfo {
                name: "Waveshare 5.65\" 7-Color ACeP".to_string(),
                code: 0x565c, 
                palette: vec![Rgb::from([006,006,006]), // Black
                              Rgb::from([240,240,240]), // White
                              Rgb::from([076,110,085]), // Green
                              Rgb::from([057,071,108]), // Blue
                              Rgb::from([166,083,079]), // Red
                              Rgb::from([221,205,094]), // Yellow
                              Rgb::from([193,102,083]), // Orange
                              Rgb::from([210,166,145])],// Tan/Clean
            },

            ScreenType::S16 => ScreenInfo {
                name: "16-Shade ePaper".to_string(),
                code: 0xAC16, 
                palette: vec![Rgb::from([0,0,0]),
                              Rgb::from([16,16,16]),
                              Rgb::from([32,32,32]),
                              Rgb::from([48,48,48]),
                              Rgb::from([64,64,64]),
                              Rgb::from([80,80,80]),
                              Rgb::from([96,96,96]),
                              Rgb::from([112,112,112]),
                              Rgb::from([128,128,128]),
                              Rgb::from([144,144,144]),
                              Rgb::from([160,160,160]),
                              Rgb::from([176,176,176]),
                              Rgb::from([192,192,192]),
                              Rgb::from([208,208,208]),
                              Rgb::from([224,224,224]),
                              Rgb::from([240,240,240]),
                              Rgb::from([255,255,255])],
            },
        }
    }
}

enum DitheringTechnique {
    FloydSteinberg,
    FloydSteinbergReducedBleed,
}

fn depalette(pixel: Rgb<u8>, screen_type: &ScreenType) -> u8
{
    let palette = screen_type.value().palette;

    for (i, x) in palette.iter().enumerate() {
        if *x == pixel {
            return i as u8;
        }
    }

    return 0;
}

fn dither_image(img: &mut RgbImage, screen_type: &ScreenType, dither: DitheringTechnique)
{
    let find_closest = |color: Rgb<u8>| -> Rgb<u8> {
        let mut closest = *screen_type.value().palette.first().unwrap();
        let mut difference : i32 = i32::max_value();
        for i in screen_type.value().palette.iter() {
            let rd : i32 = color[0] as i32 - i[0] as i32;
            let gd : i32 = color[1] as i32 - i[1] as i32;
            let bd : i32 = color[2] as i32 - i[2] as i32;
            let d = rd.pow(2) + gd.pow(2) + bd.pow(2);
            if d < difference {
                closest = *i;
                difference = d;
            }
        }

        return closest;
    };

    let apply_bias = |color: Rgb<u8>, quant: [f32; 3], bias: f32| -> Rgb<u8> {

        let mut k = [color[0] as f32, color[1] as f32, color[2] as f32];

        k[0] += quant[0] * bias;
        k[1] += quant[1] * bias;
        k[2] += quant[2] * bias;

        let newcolor = Rgb::from(k.map(|x| f32::clamp(x, 0.0, 255.0) as u8));

        return newcolor;
    };

    for y in 0..img.height() {
        for x in 0..img.width() {
            let oldpixel = *img.get_pixel(x, y);
            let newpixel = find_closest(oldpixel);
            img.put_pixel(x, y, newpixel);

            let quant : [f32; 3] = [
                oldpixel[0] as f32 - newpixel[0] as f32,
                oldpixel[1] as f32 - newpixel[1] as f32,
                oldpixel[2] as f32 - newpixel[2] as f32 
            ];

            let weight = match dither {
                DitheringTechnique::FloydSteinberg => [7.0, 5.0, 3.0, 1.0],
                DitheringTechnique::FloydSteinbergReducedBleed => [5.0, 2.0, 3.0, 1.0]
            };

            if x+1 < img.width() {
                img.put_pixel(x+1, y+0, apply_bias(*img.get_pixel(x+1, y+0), quant, weight[0]/16.0));
            }

            if y+1 < img.height() {
                img.put_pixel(x+0, y+1, apply_bias(*img.get_pixel(x+0, y+1), quant, weight[1]/16.0));

                if x > 0 {
                    img.put_pixel(x-1, y+1, apply_bias(*img.get_pixel(x-1, y+1), quant, weight[2]/16.0));
                }

                if x+1 < img.width() {
                    img.put_pixel(x+1, y+1, apply_bias(*img.get_pixel(x+1, y+1), quant, weight[3]/16.0));
                }

            }

        }
    }
}

fn modify_image(img: &RgbImage, screen_type: &ScreenType) -> Vec<u8>
{
    println!("Image dimensions: {:?}", img.dimensions());

    let mut raw : Vec<u8> = Vec::new();

    let w = img.width();
    let h = img.height();

    let mut dest = img.clone();

    dither_image(&mut dest, &screen_type, DitheringTechnique::FloydSteinbergReducedBleed);

    dest.save("/tmp/help.png").unwrap();

    let screen_code = screen_type.value().code;
    raw.push((screen_code >> 8)   as u8);
    raw.push((screen_code & 0xFF) as u8);

    for y in 0..h {
        for x in (0..w).step_by(2) {
            let px0 = depalette(*dest.get_pixel(x+0, y), &screen_type);
            let px1 = depalette(*dest.get_pixel(x+1, y), &screen_type);

            let px : u8 = ((px0 << 4) & 0xF0) | ((px1 << 0) & 0x0F);
            raw.push(px);
        }
    }

    return raw;
}

fn main()
{
    let img = match image::open("/home/andy/Downloads/porsche.jpg") {
        Ok(v) => v,
        Err(_) => {
            println!("Bad image");
            return;
        },
    };

    let target_screen = ScreenType::S565c;

    let mut dest = img.to_rgb8().clone();

    dither_image(&mut dest, &target_screen, DitheringTechnique::FloydSteinbergReducedBleed);

    dest.save("/tmp/help.png").unwrap();

    //let raw = modify_image(&img.to_rgb8(), &target_screen);

    println!("Screen Type: {}", target_screen.value().name);
    //println!("Dimensions: {:?}", raw);

}
