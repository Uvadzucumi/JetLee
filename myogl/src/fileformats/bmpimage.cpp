
#include "bmpimage.h"
#include <string.h> // strlen, strcpy
#include "../log.h"
#include <stdlib.h> // for abs()

#ifdef __WIN32__
#define GL_BGRA                           0x80E1
#endif // __WIN32__

using namespace MyOGL;

// convert pixel data to BGRA format
// using colors Shift & Mask values
void CBitMapImage::ConvertToBGRA(long int *pixel_data){
    unsigned char red, green, blue, alpha;
    // get color components
    red=(*pixel_data & m_RedMask) >> m_RedShift;
    green=(*pixel_data & m_GreenMask) >> m_GreenShift;
    blue=(*pixel_data & m_BlueMask) >> m_BlueShift;
    alpha=(*pixel_data & m_AlphaMask) >> m_AlphaShift;
    // create bgra
    ((unsigned char *)pixel_data)[3]=alpha;
    ((unsigned char *)pixel_data)[2]=red;
    ((unsigned char *)pixel_data)[1]=green;
    ((unsigned char *)pixel_data)[0]=blue;
}
/*
// calculate & return number of enabled bits in byte
short CBitMapImage::CountBits(unsigned char byte){
    short bits=0;
    for(int i=0;i<8;i++){
        if(byte%2!=0){
            bits++;
        }
        byte>>=1;
    }
    return bits;
}
*/
// calculate & return Shift value from Mask Value
short CBitMapImage::ShiftCount(long int Mask){
    if(!Mask) return 0;
    int tmp=0;
    while(Mask%2 == 0){
        tmp++;
        Mask>>=1;
    }
    //return tmp - (8 - CountBits(Mask && 0xFF));
    return tmp;
}

// load bitmap image from file
bool CBitMapImage::LoadFromFile(const char *file_name){
    m_file_name=new char[strlen(file_name)];
    strcpy(m_file_name,file_name);

    FILE *fp = fopen(file_name, "rb");
    if (fp==NULL) { Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Can't open %s\n", file_name); return false; }

    // set the file pointer to end of file
    fseek(fp, 0, SEEK_END);
    // get file size
    int file_size=ftell(fp);
    // set the file pointer to start of file
    //fseek( fp, 0, SEEK_SET );
    rewind(fp);

    // read file header
    if(fread(&m_file_header,1,sizeof(m_file_header),fp)!=sizeof(m_file_header)){
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Rread bitmap File header error!\n",m_file_name);
        fclose(fp);
        return false;
    };
    // check bitmap and file size
    if(m_file_header.magic[0]!='B' || m_file_header.magic[1]!='M'){
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Wrong BMP File Format!\n",m_file_name);
        fclose(fp);
        return false;
    }
    // check bmp file size
    // check file size & readed from header
    if(m_file_header.file_size!=file_size){
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Wrong BMP File Header Size! header.size:%d != filesystem.size:%d\n",file_name, m_file_header.file_size,file_size);
        fclose(fp);
        return false;
    }

    // read bitmap info header
    if(fread(&m_info_header,1,sizeof(m_info_header),fp)!=sizeof(m_info_header)){
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Rread bitmap Info header error!\n",file_name);
        fclose(fp);
        return false;
    }

    // check compression
    if(
       (m_info_header.compression==bcBI_RLE4 && m_info_header.bpp!=4) ||
       (m_info_header.compression==bcBI_RLE8 && m_info_header.bpp!=8) ||
       (m_info_header.compression==bcBI_BITFIELDS && ( m_info_header.bpp!=16 && m_info_header.bpp!=32)) ||
       (m_info_header.compression!=bcBI_RGB && m_info_header.compression!=bcBI_RLE4 && m_info_header.compression!=bcBI_RLE8 && m_info_header.compression!=bcBI_BITFIELDS)
       ){ // bad compression
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Bad BMP Compression method %d!\n", file_name, m_info_header.compression);
        fclose(fp);
        return false;
    }
    m_top_down=(m_info_header.height < 0);
    m_info_header.height=abs(m_info_header.height);
    if(m_top_down && (m_info_header.compression != bcBI_RGB || m_info_header.compression != bcBI_BITFIELDS)){
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") top down bmp cannot be compressed!\n",m_file_name);
        fclose(fp);
        return false;
    }

    if(m_info_header.compression == bcBI_RGB && m_info_header.bpp==16){ // 5 bits per channel, fixed mask
        Log->puts("DEBUG: Fixed map BMP image (5 bits)\n");
        m_RedMask = 0x7C00;
        m_RedShift = 7;
        m_GreenMask = 0x03E0;
        m_GreenShift = 2;
        m_BlueMask = 0x001F;
        m_BlueShift = -3;
    }else if(m_info_header.compression == bcBI_BITFIELDS && (m_info_header.bpp == 16 || m_info_header.bpp == 32)){ // arbitrary mask
        Log->puts("DEBUG: Arbitrary map BMP image\n");
        // read mask from file
        fread(&m_RedMask,1,4,fp);
        fread(&m_GreenMask,1,4,fp);
        fread(&m_BlueMask,1,4,fp);
        if(m_info_header.bpp==32){ fread(&m_AlphaMask,1,4,fp); }

        m_RedShift = ShiftCount(m_RedMask);
        m_GreenShift = ShiftCount(m_GreenMask);
        m_BlueShift = ShiftCount(m_BlueMask);
        m_AlphaShift = ShiftCount(m_AlphaMask);
        Log->printf("DEBUG: RMask: %x GMask: %x BMask: %x AMask: %x\n",m_RedMask, m_GreenMask, m_BlueMask, m_AlphaMask);
        Log->printf("DEBUG: RShift: %d GShift: %x BShift: %d AShift: %d\n",m_RedShift, m_GreenShift, m_BlueShift, m_AlphaShift);
    }else if(m_info_header.bpp==1 || m_info_header.bpp==4 || m_info_header.bpp==8){
        Log->puts("DEBUG: BMP with palette\n");
        m_palette_size=1 << m_info_header.bpp;
        Log->printf("DEBUG: Palette seize=%d\n", m_palette_size);
        m_palette=new GLPixel32 [m_palette_size];
        // read palette
        if(m_info_header.colors_used){
            // read colors
            fread(&m_palette,m_info_header.colors_used,sizeof(GLPixel32),fp);
        }else{ // read colors (numbers - calculated)
            Log->printf("Warning: used calculated palette size!");
            fread(&m_palette,m_palette_size,sizeof(GLPixel32),fp);
        }
    }
    fseek(fp, m_file_header.offset, SEEK_SET); // go to start pixel data
// file header debug
    Log->printf("DEBUG: file_name=%s file_size=%d\n", m_file_name, m_file_header.file_size);
    Log->printf("DEBUG: magic=%c%c offset=%d\n",m_file_header.magic[0],m_file_header.magic[1],m_file_header.offset);
// info header debug
    Log->printf("DEBUG: if.size=%d img.w=%d img.h=%d img.planes=%d img.bpp=%d\n",m_info_header.size,
              m_info_header.width, m_info_header.height, m_info_header.planes, m_info_header.bpp);
    Log->printf("DEBUG: if.compression=%d if.image_size=%d if.x_ppm=%d if.y_ppm=%d\n",
                m_info_header.compression, m_info_header.image_size, m_info_header.x_pixs_per_meter, m_info_header.y_pixs_per_meter);
    Log->printf("DEBUG: if.colors_used=%d if.colors_important=%d\n", m_info_header.colors_used, m_info_header.colors_important);

    // read data (now only BMP32)
    if(m_info_header.bpp==32){
        m_pixel_format=GL_BGRA;
        m_bytes_prer_pixel=4;
        unsigned int scanline_size=m_bytes_prer_pixel*m_info_header.width;
        int pixel_data_size=scanline_size*m_info_header.height;
    // allocate mem
        if(m_pixel_data){ // free mem from old data
            delete m_pixel_data;
        }
        m_pixel_data=new unsigned char [pixel_data_size];
        //unsigned char* read_buffer=new unsigned char [scanline_size];
        for(int row=m_info_header.height-1; row >= 0; row--){
            if(fread(&m_pixel_data[row*scanline_size], 1, scanline_size, fp) != scanline_size){
                Log->printf("Error read scan line # %d\n", m_info_header.height-row);
            };
            //memcpy(&m_pixel_data[row*scanline_size], read_buffer, scanline_size);
        }
        if(m_info_header.compression==bcBI_BITFIELDS){ // need convert to BGRA
            for(int row=0;row<m_info_header.height;row++){
                for(int pix=0;pix<m_info_header.width;pix++){
                    ConvertToBGRA((long int *)(&m_pixel_data[row*scanline_size+pix*4]));
                }
            }
        }
    }else{
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Unsupported bitmap Format!\n",m_file_name);
        fclose(fp);
        return false;
    }
    fclose(fp);
    Log->printf("DEBUG: BMP FILE READED!\n");
    return true;
}

// copy pixel data to data pointer and return address
int CBitMapImage::GetPixelData(std::vector<unsigned char>& data){
    if(data.size()){
        Log->puts("Warning: CBitMapImage::GetPixelData() pinter not NULL, previous data deleted\n");
        data.clear();
    }
    int pixel_data_size=m_bytes_prer_pixel*m_info_header.width*m_info_header.height;
    // allocate mem
    data.resize(pixel_data_size);
    memcpy(&data[0],m_pixel_data,pixel_data_size);
    return 0;
}
