#ifndef BMPIMAGE_H_INCLUDED
#define BMPIMAGE_H_INCLUDED

    #include <stdio.h>
    #include <GL/gl.h>

    #include <vector>

/* Load bmp image, return collor butes array */
namespace MyOGL{

    enum enBmpCompression{
        bcBI_RGB=0,
        bcBI_RLE4=1,
        bcBI_RLE8=2,
        bcBI_BITFIELDS=3,
        bcBI_JPEG=4,
        bcBI_PNG=5,
        bcBI_ALPHABITFIELDS=6
    };

    struct sBitMapFileHeader{
        char magic[2];              // 0: File type (2 byte)
        long int file_size;         // 2: File size in bytes (4 byte)
        long int reserved;          // 6: Reserved (4 byte)
        long int offset;            // 10: image data offset (sizeof(file hieder + the info header + palette)) (4 byte)
    } __attribute__((packed));

    struct sBitMapInfoHeader{
        long int size;              // 0(14): info header size: always=40 (0x28h) (4 byte)
        long int width;             // 4(18): image width (4 byte)
        long int height;            // 8(22): image height (4 byte)
        short int planes;           // 12(26): image planes: always=1 (2 byte)
        short int bpp;              // 14(28): color resolution: bits per pixel (1,4,8,16,24,32) (2 byte)
        long int compression;       // 16(30): compression type (4 bytes)
        long int image_size;        // 20(34): pixel data size. 0 - if not compressed (4 byte)
        long int x_pixs_per_meter;  // 24(38): Horizontal resolution in pixel/meter (4 byte)
        long int y_pixs_per_meter;  // 28(42): Vertical resolution in pixel/meter (4 byte)
        long int colors_used;       // 32(46): Number of colors used (4 bytes)
        long int colors_important;  // 36(50): Number of imprtant colors used : usefull for displaying on VGA256 (4 bytes)
    } __attribute__((packed));

    struct GLPixel32{
        unsigned char red,green,blue,alpha;
    } __attribute__((packed));

    class CBitMapImage{
            sBitMapFileHeader m_file_header;
            sBitMapInfoHeader m_info_header;
            char *m_file_name;
            bool m_top_down;
            int m_palette_size;
            GLPixel32 *m_palette;
            unsigned char *m_pixel_data;
            GLuint m_pixel_format; // GL_BGR, GL_BGRA
            int m_bytes_prer_pixel;
            // TODO: moved to CBaseImage?
            long int m_RedMask, m_GreenMask, m_BlueMask, m_AlphaMask;
            short m_RedShift, m_GreenShift, m_BlueShift, m_AlphaShift;

            void ConvertToBGRA(long int *pixel_data);
            //short CountBits(unsigned char byte); // calculate & return coun of enabled bits in byte
            short ShiftCount(long int Mask); // calculate & return shift from mask

        public:
            CBitMapImage(){
                m_palette_size=0;
                m_file_name=NULL;
                m_palette=NULL;
                m_pixel_data=NULL;
            };
            ~CBitMapImage(){
                if(m_file_name) delete m_file_name;
                if(m_palette) delete m_palette;
                if(m_pixel_data) delete m_pixel_data;
                m_file_name=NULL;
                m_palette=NULL;
                m_pixel_data=NULL;
            };
            bool LoadFromFile(const char *file_name);
            // getters
            int GetWidth(){ return m_info_header.width; };
            int GetHeight(){ return m_info_header.height; };
            GLuint GetPixelFormat(){ return m_pixel_format; };
            int GetBytesPerPixel(){ return m_bytes_prer_pixel; }
            // copy pixel data to pointer *data and return pointer address
            int GetPixelData(std::vector<unsigned char>& data);
    };

}

#endif // BMPIMAGE_H_INCLUDED
