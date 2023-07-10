#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BBP = 32;

SDL_Surface *g_object = NULL;
SDL_Surface *g_screen = NULL;
SDL_Surface *g_bkground = NULL;
SDL_Event g_even;

// Hàm LoadImage nhận đường dẫn tệp tin hình ảnh và trả về một SDL_Surface*
SDL_Surface *LoadImage(std::string file_path)
{
    SDL_Surface *load_image = NULL;
    SDL_Surface *optimize_image = NULL;

    // Sử dụng IMG_Load của SDL_image để tải hình ảnh từ đường dẫn
    load_image = IMG_Load(file_path.c_str());

    // Kiểm tra xem việc tải hình ảnh có thành công hay không
    if (load_image != NULL)
    {
        // Tối ưu hóa hình ảnh để phù hợp với định dạng hiển thị trên màn hình
        optimize_image = SDL_DisplayFormat(load_image);

        // Giải phóng bộ nhớ của hình ảnh đã tải
        SDL_FreeSurface(load_image);
        if (optimize_image != NULL)
        {
            // Thiết lập màu trong suốt (color key) để loại bỏ phần nền không mong muốn
            Uint32 color_key = SDL_MapRGB(optimize_image->format, 0, 0xFF, 0xFF);
            SDL_SetColorKey(optimize_image, SDL_SRCCOLORKEY, color_key);
        }
    }

    // Trả về hình ảnh đã tải và được tối ưu
    return optimize_image;
}

bool Init()
{
    // Khởi tạo SDL và các thành phần liên quan
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return false;

    // Thiết lập chế độ video với kích thước màn hình, độ sâu màu và kiểu surface
    g_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BBP, SDL_SWSURFACE);
    if (g_screen == NULL)
        return false;

    // Trả về true nếu khởi tạo thành công
    return true;
}

void ApplySurface(SDL_Surface *src, SDL_Surface *des, int x, int y)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(src, NULL, des, &offset);
}

void Cleanup()
{
    SDL_FreeSurface(g_screen);
    SDL_FreeSurface(g_bkground);
}

int main(int arc, char *argv[])
{
    bool is_quit = false;
    if (Init() == false)
        return 0;

    // Tải và hiển thị hình nền
    g_bkground = LoadImage("imggame/bg2.png");
    if (g_bkground == NULL)
    {
        return 0;
    }
    ApplySurface(g_bkground, g_screen, 0, 0);

    // Tải và hiển thị đối tượng
    g_object = LoadImage("imggame/human64x91.png");
    if (g_object == NULL)
        return 0;
    ApplySurface(g_object, g_screen, 300, 420);

    while (!is_quit)
    {
        while (SDL_PollEvent(&g_even))
        {
            if (g_even.type == SDL_QUIT)
            {
                is_quit = true;
                break;
            }
        }
        if (SDL_Flip(g_screen) == -1)
            return 0;
    }

    Cleanup();
    SDL_Quit();
    return 1;
}
