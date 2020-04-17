#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

void PerlinNoise2D(int nWidth, int nHeight, float *fSeed, int nOctaves, float fBias, float *fOutput)
{
    for (int x = 0; x < nWidth; x++)
	for (int y = 0; y < nHeight; y++)
	{				
		float fNoise = 0.0f;
		float fScaleAcc = 0.0f;
		float fScale = 1.0f;

		for (int o = 0; o < nOctaves; o++)
		{
			int nPitch = nWidth >> o;
			int nSampleX1 = (x / nPitch) * nPitch;
			int nSampleY1 = (y / nPitch) * nPitch;
					
			int nSampleX2 = (nSampleX1 + nPitch) % nWidth;					
			int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

			float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
			float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

			float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY1 * nWidth + nSampleX2];
			float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY2 * nWidth + nSampleX2];

			fScaleAcc += fScale;
			fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
			fScale = fScale / fBias;
		}

		// Scale to seed range
		fOutput[y * nWidth + x] = fNoise / fScaleAcc;
	}
	
}

int main() {
	srand (time(NULL));

	int nOutputWidth = 32;
	int nOutputHeight = 32;
	float *fNoiseSeed2D = nullptr;
	float *fPerlinNoise2D = nullptr;

	fNoiseSeed2D = new float[nOutputWidth * nOutputHeight];
	fPerlinNoise2D = new float[nOutputWidth * nOutputHeight];
    for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float) rand() / (float) RAND_MAX;
    
    for (int i = 0; i < nOutputWidth; i++) {
        fNoiseSeed2D[i] = 0;
        fNoiseSeed2D[i + (nOutputHeight-1) * nOutputWidth] = 0;
    }
    
    for (int i = 0; i < nOutputHeight; i++) {
        fNoiseSeed2D[i * nOutputWidth] = 0;
        fNoiseSeed2D[i * nOutputWidth + (nOutputWidth-1)] = 0;
    }
    
	PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, std::log2(nOutputWidth), 2.f, fPerlinNoise2D);

	//for (int i = 0; i < nOutputWidth * nOutputHeight; i++) std::cout << fPerlinNoise2D[i] << std::endl;
    
    
    // create the window
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "My window", sf::Style::Titlebar);
    
    sf::Image image;
    image.create(nOutputWidth, nOutputHeight, sf::Color(0, 0, 0));
    sf::Texture texture;
    sf::Sprite sprite;
    
    int factor = 12;
    
    for(int x = 0; x < nOutputWidth; x++)
    {
        for(int y = 0; y < nOutputHeight; y++)
        {
            if (fPerlinNoise2D[y * nOutputWidth + x] * factor > 5) {
                image.setPixel(x, y, sf::Color(255, 255, 255));
            } else if (fPerlinNoise2D[y * nOutputWidth + x] * factor > 4) {
                image.setPixel(x, y, sf::Color(211,211,211));
            } else if (fPerlinNoise2D[y * nOutputWidth + x] * factor > 3) {
                image.setPixel(x, y, sf::Color(169,169,169));
            } else if (fPerlinNoise2D[y * nOutputWidth + x] * factor > 2) {
                image.setPixel(x, y, sf::Color(128,128,128));
            } else if (fPerlinNoise2D[y * nOutputWidth + x] * factor > 1) {
                image.setPixel(x, y, sf::Color(105,105,105));
            } else {
                image.setPixel(x, y, sf::Color(0,0,0));
            }
        }
    }
    
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setScale(1024/nOutputWidth, 1024/nOutputHeight);

    window.draw(sprite);
        
    // end the current frame
    window.display();
    
    // run the program as long as the window is open
    while (window.isOpen())
    {

            
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                {
                    srand (time(NULL));
                    std::cout << time(NULL) << std::endl;
                    
                    for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float) rand() / (float) RAND_MAX;
                    
                    for (int i = 0; i < nOutputWidth; i++) {
                        fNoiseSeed2D[i] = 0;
                        fNoiseSeed2D[i + (nOutputHeight-1) * nOutputWidth] = 0;
                    }
                    
                    for (int i = 0; i < nOutputHeight; i++) {
                        fNoiseSeed2D[i * nOutputWidth] = 0;
                        fNoiseSeed2D[i * nOutputWidth + (nOutputWidth-1)] = 0;
                    }
                    
                    PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, std::log2(nOutputWidth), 2.f, fPerlinNoise2D);
                    
                    for(int x = 0; x < nOutputWidth; x++)
                    {
                        for(int y = 0; y < nOutputHeight; y++)
                        {
                            if (fPerlinNoise2D[y * nOutputWidth + x] * factor > 5) {
                                image.setPixel(x, y, sf::Color(255, 255, 255));
                            } else if (fPerlinNoise2D[y * nOutputWidth + x] * factor > 4) {
                                image.setPixel(x, y, sf::Color(211,211,211));
                            } else if (fPerlinNoise2D[y * nOutputWidth + x] * factor > 3) {
                                image.setPixel(x, y, sf::Color(169,169,169));
                            } else if (fPerlinNoise2D[y * nOutputWidth + x] * factor > 2) {
                                image.setPixel(x, y, sf::Color(128,128,128));
                            } else if (fPerlinNoise2D[y * nOutputWidth + x] * factor > 1) {
                                image.setPixel(x, y, sf::Color(105,105,105));
                            } else {
                                image.setPixel(x, y, sf::Color(0,0,0));
                            }
                        }
                    }
                    
                    texture.loadFromImage(image);
                    sprite.setTexture(texture);
                    window.draw(sprite);
                    
                        
                    // end the current frame
                    window.display();
                }
            }
                
        }
        

    }
    return 0;
}
