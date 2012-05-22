//
//  tgaTexureLoader.h
//  gles20farm
//
//  Created by astojilj on 3/24/12.
//  copied logic from akTextureLoader, just here loading from file. 
//

#ifndef gles20farm_tgaTexureLoader_h
#define gles20farm_tgaTexureLoader_h

#include "utTypes.h"
#include "utStreams.h"

#include "stdlib.h"

#ifdef OPENGL_ES_2_0
#ifndef GL_BGRA
#define GL_BGRA GL_RGBA // would be incorrect, but visibly incorrect
#endif
#endif

#ifndef GL_BGR
#define GL_BGR GL_RGB
#endif

namespace tgaTexureLoader {

//Header containing information on the image data and palette
//Optional image identification field
//Optional color map
//Bitmap data

typedef struct TgaHeader
{
	UTbyte   IDLength;        /* 00h  Size of Image ID field */
	UTbyte   ColorMapType;    /* 01h  Color map type */
	UTbyte   ImageType;       /* 02h  Image type code */
	UTbyte   CMapStart[2];    /* 03h  Color map origin */
	UTbyte   CMapLength[2];   /* 05h  Color map length */
	UTbyte   CMapDepth;       /* 07h  Depth of color map entries */
	UTuint16 XOffset;         /* 08h  X origin of image */
	UTuint16 YOffset;         /* 0Ah  Y origin of image */
	UTuint16 Width;           /* 0Ch  Width of image */
	UTuint16 Height;          /* 0Eh  Height of image */
	UTbyte   PixelDepth;      /* 10h  Image pixel size */
	UTbyte   ImageDescriptor; /* 11h  Image descriptor UTbyte */
} TgaHeader;

bool checkHeader(TgaHeader& head)
{
	if(head.ColorMapType != 0)
		return false;
	if(head.ImageType != 2)
		return false;
	if(head.XOffset != 0)
		return false;
	if(head.YOffset != 0)
		return false;
	if(head.PixelDepth != 32 && head.PixelDepth != 24)
		return false;
	return true;
}

GLuint loadRawTargaTexture(const char *filename)
{
    utFileStream stream;
    stream.open(filename, utStream::SM_READ);

    GLuint texture;
    TgaHeader header;
	UTuint32 size;
	void* data;
    
    //	Debug should be 18
    //	UTuint32 hsize = sizeof(TgaHeader); 
    
	stream.read(&header, sizeof(TgaHeader));
	
	if(!checkHeader(header))
		return 0;
	
	if(header.IDLength)
		stream.seek(header.IDLength, SEEK_CUR);
	
	size = header.Width * header.Height * header.PixelDepth / 8;
	data = malloc(size);
	stream.read(data, size);
	
    // iPhone supports GL_BGRA but only as input format - GL_RGBA needs to be supplied as internal
	GLint intform = GL_RGBA;
	GLint form = GL_BGRA;
	if(header.PixelDepth == 24)
	{
		intform = 3;
        form = GL_BGR;
    }
    
    
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	
    glTexImage2D(GL_TEXTURE_2D, 0, intform, header.Width, header.Height, 0, form, GL_UNSIGNED_BYTE, data);
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
	free(data);
	return texture;
}

}

#endif
