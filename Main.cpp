#include <iostream>
#include <cmath>
#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"
#include <vector>

// Whats wrong

// event handler for keybord is not responsive
// The framer is somehow have uninitialized value ( Animation also messed up this process)

//Currently doing



using namespace std;

//Global flags

static bool active = false;

namespace alg{
float com_max(float a, float b, float c, float d)
    {

        float angka[4] = {a, b, c, d};
        float banyak = 4;

        float besar = a;



            for(int n = 0; n < banyak; n++)
            {
                if(besar < angka[n])
                {
                    besar = angka[n];
                }else{}



            }

        return besar;
    }

    float com_min(float a, float b, float c, float d)
    {

        float angka[4] = {a, b, c, d};
        float banyak = 4;

        float kecil = a;



            for(int n = 0; n < banyak; n++)
            {
                if(kecil > angka[n])
                {
                    kecil = angka[n];
                }else{}



            }

    }
};

class shape
{
public:
    char name;                // r = rectangle,  c = circle, l = line
};

class rectangle: public shape
{
public:

    int x;
    int y;
    int w;
    int h;

    void init(int awalx, int awaly, int lebar, int tinggi)
    {
        x = awalx;
        y = awaly;
        w = lebar;
        h = tinggi;
    }
};

class animation
{
private:
    SDL_Texture* pMainImage; // Sumber frame-frame animasi
    const int w;             //dimension of a frame
    const int h;
    int frame;
    int maxframe;
    SDL_Rect FramingRectangle;
    SDL_Renderer* pRenderer;

public:
    animation(int width, int height, std::string bitmap, SDL_Renderer* pRender):w(width), h(height)
    {
        frame = 1;
        SDL_Surface* psurface = SDL_LoadBMP(bitmap.c_str());
        pMainImage = SDL_CreateTextureFromSurface(pRender, psurface);
        SDL_FreeSurface(psurface);
        pRenderer = pRender;

        int fw;  // dimension of the source image
        int fh;

        SDL_QueryTexture(pMainImage, NULL, NULL, &fw, &fh); // Obtaining the dimension of the source image

        maxframe = (std::max(fw / width, fh / height));  // determining the number of available frame;

        FramingRectangle.x = 0;
        FramingRectangle.y = 0;              // Determining the frame that is used to splice a frame from the source image
        FramingRectangle.w = w;
        FramingRectangle.h = h;
    }




    SDL_Texture * pSource()
    {
        return pMainImage;
    }

    SDL_Rect * pSplicing()
    {
        return &FramingRectangle;
    }

    void advance_frame(int &frame)      // Changing the value of variable that hold the position of current frame
    {
        frame++;
        if(frame > maxframe){frame = 1;}
        else if(frame < 1){frame = maxframe;}
        else{}
    }

    void getFramingRectangle(SDL_Rect & kotak, int current_frame)  // Chaning Framing Rectangle according to current frame
    {
        FramingRectangle.x = (current_frame - 1) * w;


        kotak.w = FramingRectangle.w;
        kotak.h = FramingRectangle.h;
        kotak.x = FramingRectangle.x;
        kotak.y = FramingRectangle.y;
    }

    SDL_Texture* getTexturePointer()
    {
        return pMainImage;
    }

    SDL_Renderer* getPointerToRenderer()
    {
        return pRenderer;

    }
    int getWidth()
    {
        return w;
    }

    int getHeight()
    {
        return h;
    }


};


class object
{
    public: char name;
};


class entity: public object
{
private:
    rectangle bentuk;                     // Hitbox dari entity
    int x;                                // Lokasi entity (merepresentasikan lokasi titik tengah dari entity)
    int y; // Lokasi  (Di tengah objek)
    int health;
    SDL_Texture* pTexture;                // Pointer kepada tesktur yang akan digunakan
    SDL_Renderer* pRenderer;              // Pointer kepada OpenGL context (SDL_Renderer) mana yang akan digunakan oleh entity ini untuk renddring
    SDL_Rect framertexture;               // Framer untuk menentukan bagian mana dari tekstur yang aakan dirender
    SDL_Rect sceneposition;               // Framer yang akan digunakan untuk menentukan dimana tekstur akan dirender

    // Untuk fitur attach ke animasi
    animation* animasi;                   // Pointer kepada animasi yang akan digunakan
    int current_frame;

public:


    void mold_sdl_rect(SDL_Rect & kotak)    // Membentu SDL_Rect sesuai dengan bentuk entity ini
    {
        kotak.x = bentuk.x;
        kotak.y = bentuk.y;
        kotak.w = bentuk.w;
        kotak.h = bentuk.h;
    }

    entity(int lokasix, int lokasiy, SDL_Renderer* pointertorenderer, int darah = 100, int lebar = 50, int tinggi = 50)
    {
        x = lokasix;
        y = lokasiy;
        health = darah;
        pRenderer = pointertorenderer;
        bentuk.init(x - 1/2 * lebar, y - 1/2 * tinggi, lebar, tinggi);
        mold_sdl_rect(sceneposition);
        current_frame = 1;
        framertexture.h = 50;
        framertexture.w = 50;
        framertexture.x = 0;
        framertexture.y = 0;
    }

    void AttachTexture(SDL_Texture* blyat)  // Memberikan texture pada entity
    {
        pTexture = blyat;

    }
    void LoadBMP(std::string nama)           // Memberikan texture pada entity
    {
        SDL_Surface* cyka = SDL_LoadBMP(nama.c_str());
        pTexture = SDL_CreateTextureFromSurface(pRenderer, cyka);
        SDL_FreeSurface(cyka);
    }
    SDL_Rect sdlrect()                       // mengembalikan SDL_Rect sesuai dengan bentuk, entah buat apa
    {
        SDL_Rect kotak;
        kotak.h = bentuk.h;
        kotak.w = bentuk.w;
        kotak.x = bentuk.x;
        kotak.y = bentuk.y;
        return kotak;
    }

    void traverse(int vectorx, int vectory)
    {
        x += vectorx;
        y += vectory;

        bentuk.init(x - 1/2 * bentuk.w, y - 1/2 * bentuk.h, bentuk.w, bentuk.h);

        mold_sdl_rect(sceneposition);
    }

    void teleport(int xx, int yy)
    {
        x = xx;
        y = yy;

        bentuk.init(x - 1/2 * bentuk.w, y - 1/2 * bentuk.h, bentuk.w, bentuk.h);

        mold_sdl_rect(sceneposition);
    }



    int posx()
    {
        return x;
    }

    int posy()
    {
        return y;
    }


    SDL_Texture* pointerketexture()
    {
        return pTexture;
    }

    SDL_Rect* pointerkeframertexture()
    {
        return &framertexture;
    }

    SDL_Rect* pointerkesceneposition()
    {
        return &sceneposition;
    }

    SDL_Renderer* pointerkerenderer()
    {
        return pRenderer;
    }

    void AttachAnimation(animation& anim)
    {
        if(pRenderer == anim.getPointerToRenderer())
        {
            animasi = &anim;
            framertexture.w = anim.getWidth();
            framertexture.w = anim.getHeight();
            framertexture.y = 0;
            pTexture = anim.getTexturePointer();
        }
        else{}

    }

    void getAnimation()
    {
        animasi->getFramingRectangle(framertexture, current_frame);
       // pTexture = animasi->getTexturePointer();
    }

    void animate()
    {
        animasi->advance_frame(current_frame);
        animasi->getFramingRectangle(framertexture, current_frame);
       // pTexture = animasi->getTexturePointer();
    }




};



bool isColliding_AABB(rectangle kotak1, rectangle kotak2)   // Axis Aligned Bounding Box
{
    int x1 = kotak1.x;
    int x2 = kotak1.x + kotak1.w;
    int x3 = kotak2.x;
    int x4 = kotak2.x + kotak2.w;

    int y1 = kotak1.y;
    int y2 = kotak1.y + kotak1.h;
    int y3 = kotak2.y;
    int y4 = kotak2.y + kotak2.h;

    int xmin = alg::com_min(x1, x2, x3, x4);
    int xmax = alg::com_max(x1, x2, x3, x4);

    int ymin = alg::com_min(y1, y2, y3, y4);
    int ymax = alg::com_max(y1, y2, y3, y4);

    int xray = std::abs(xmin - xmax);
    int yray = std::abs(ymin - ymax);

    if((xray <= (kotak1.w + kotak2.w)) && (yray <= (kotak1.h + kotak2.h))){return true;}
    else{return false;}


}







class RenderWrapper
{
private:
    std::vector<SDL_Texture*> PointerToStaticTextures;         // texture that usually persist, but can be changed manually. Used for static object
    std::vector<SDL_Rect*> pStaticTexturesFraming;             // Frame for determining which part of the textures should be rendered
    std::vector<SDL_Rect*> pStaticTexturesAllocator;           // Frame for determining where should a texture be rendered

    SDL_Texture* pointertostatic;
    SDL_Rect* pointertoframer;
    SDL_Rect* pointertoallocator;


    std::vector<SDL_Texture*> PointerToTextures;             // Dynamic texture, cleared out every frame; used for frequently moving object
    std::vector<SDL_Rect> TexturesFraming;

    SDL_Rect camera;

    SDL_Renderer* pRenderer;


public:



    RenderWrapper(SDL_Renderer* renderer, int camera_x_starting_point, int camera_y_starting_point, int camera_width, int camera_height)
    {
        pRenderer = renderer;

        camera.x = camera_x_starting_point;
        camera.y = camera_y_starting_point;
        camera.w = camera_width;
        camera.h = camera_height;
    }




    bool attach_to_static(entity& orang)                  // Menempelkan identitas grafis ke RenderWrapper
    {

        if(orang.pointerkerenderer() == pRenderer)
        {
            PointerToStaticTextures.push_back(orang.pointerketexture());
            pStaticTexturesFraming.push_back(orang.pointerkeframertexture());
            pStaticTexturesAllocator.push_back(orang.pointerkesceneposition());

            /*pointertostatic = orang.pointerketexture();
            pointertoframer = orang.pointerkeframertexture();
            pointertoallocator = orang.pointerkesceneposition();*/
            return true;
        }else
        {
            return false;
        }

    }

    void render()
    {
        // clearing buffer
        SDL_RenderClear(pRenderer);

        // filling buffer
        for(int n = 0; n < PointerToStaticTextures.capacity(); n++)
        {
            SDL_RenderCopy(pRenderer, PointerToStaticTextures[n], pStaticTexturesFraming[n], NULL);
           //SDL_RenderCopy(pRenderer, PointerToStaticTextures[n], pStaticTexturesFraming[n], NULL);
           // SDL_RenderCopy(pRenderer, pointertostatic, pointertoframer, NULL);
        }

        // Rendering
        SDL_RenderPresent(pRenderer);

    }




    void debug_static_textures_memory()
    {
        for(int n = 0; n < PointerToStaticTextures.capacity(); n++)                   // Printing all textures into the console  (requires no loop)
        {
            std::cout << PointerToStaticTextures[n] << std::endl;
        }
    }

    void debug_static_framing()
    {
        for(int n = 0; n < pStaticTexturesFraming.capacity(); n++)                   // Printing all textures into the console  (requires no loop)
        {
            std::cout << pStaticTexturesFraming[n]->w << "  " << pStaticTexturesFraming[n]->h << "  " << pStaticTexturesFraming[n]->x << "  " << pStaticTexturesFraming[n]->y << std::endl;
        }
    }


    SDL_Texture* getTexture(int n)
    {
        return PointerToStaticTextures[n];
    }

    SDL_Rect* getFramer(int n)
    {
        return pStaticTexturesFraming[n];
    }




};













int main(int, char **)
{

    SDL_Init(SDL_INIT_EVERYTHING);

    auto pWindow = SDL_CreateWindow("Just a Normal Day", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

    if(pWindow != nullptr){}  // Jika pembuatan window sukses
    else{}                    // Jika pembutaan window gagal

    SDL_Surface * pSurface = SDL_LoadBMP("Jlob.bmp");
    if(pSurface != nullptr){cout << "berhasil load ke vram" << endl;} // Jika loading ke vram berhasil
    else{}                    // Jika loading ke vram gagal

    auto pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);



    SDL_Surface * pmapsur = SDL_LoadBMP("map.bmp");
    SDL_Texture * pMapTexture = SDL_CreateTextureFromSurface(pRenderer, pmapsur);
    SDL_FreeSurface(pmapsur);



    static SDL_Event event;
    static bool KEY_W = false;
    static bool KEY_A = false;
    static bool KEY_S = false;
    static bool KEY_D = false;



    animation blyat(50, 50, "animation.bmp", pRenderer);

    static Uint32 period;
    static Uint32 start_tick;
    static Uint32 finish_tick;

    entity pirel(200, 200, pRenderer);
    entity direl(300, 300, pRenderer);

    RenderWrapper gl(pRenderer, 0, 0, 640, 480);

    pirel.LoadBMP("map.bmp");
    pirel.AttachAnimation(blyat);
    //pirel.LoadBMP("animation.bmp");


    std::cout << gl.attach_to_static(pirel) << std::endl;

    std::cout << "Pointer sebelum animasi" << std::endl;

    gl.debug_static_textures_memory();
    std::cout << std::endl;
    gl.debug_static_framing();
    std::cout << pirel.pointerketexture() << std::endl;
    std::cout << pirel.pointerkeframertexture() << std::endl;


    pirel.getAnimation();
    pirel.animate();
    pirel.animate();
    pirel.animate();

    std::cout << "Pointer setelah animasi" << std::endl;

    gl.debug_static_textures_memory();
    std::cout << std::endl;
    gl.debug_static_framing();
    std::cout << pirel.pointerketexture() << std::endl;
    std::cout << pirel.pointerkeframertexture() << std::endl;




    while(true)
    {
        /*//Pre processing
        period = finish_tick - start_tick;

        start_tick = SDL_GetTicks();




        system("cls");

        //Input
        KEY_W = false;
        KEY_A = false;
        KEY_S = false;
        KEY_D = false;


        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_w){KEY_W = true;}
                if(event.key.keysym.sym == SDLK_a){KEY_A = true;}
                if(event.key.keysym.sym == SDLK_s){KEY_S = true;}
                if(event.key.keysym.sym == SDLK_d){KEY_D = true;}
            }else
            {

            }

        }







        //Simulation

        if(KEY_W){pirel.change_pos(pirel.x, pirel.y - 2);}
        if(KEY_S){pirel.change_pos(pirel.x, pirel.y + 2);}
        if(KEY_A){pirel.change_pos(pirel.x - 2, pirel.y);}
        if(KEY_D){pirel.change_pos(pirel.x + 2, pirel.y);}

        blyat.nextFrame();

        /*std::cout << pirel.x << "    " << pirel.y << std::endl;
        std::cout << KEY_A << std::endl;
        std::cout << KEY_W << std::endl;
        std::cout << KEY_S << std::endl;
        std::cout << KEY_D << std::endl;*/





/*
        //Render
        SDL_RenderClear(pRenderer);

        SDL_RenderCopy(pRenderer, pMapTexture, NULL, NULL);

        SDL_RenderCopy(pRenderer, blyat.pSource(), blyat.pSplicing(), NULL);


        pirel.mold_sdl_rect(kotak);
        SDL_RenderCopy(pRenderer, pirel.pTexture, NULL, &kotak);

        SDL_RenderPresent(pRenderer);

        //Post Processing
        SDL_Delay(200);

        finish_tick = SDL_GetTicks();

        std::cout << period << std::endl;*/



        gl.render();



        /*SDL_Rect kotak1;
        SDL_Rect kotak2;
        blyat.getFramingRectangle(kotak1, 3);
        SDL_RenderClear(pRenderer);

        SDL_RenderCopy(pRenderer, blyat.getTexturePointer(), &kotak1, NULL);

        SDL_RenderPresent(pRenderer);*/









    }
    std::cout << SDL_GetError() << std::endl;

    return 0;


}
