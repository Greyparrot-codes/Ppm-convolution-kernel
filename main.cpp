#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <raylib.h>

using namespace std;


void clamp(int& value, int minimum, int maximum){
    if(value >= maximum) {value = maximum;}
    if(value <= minimum) {value = minimum;}
}

struct color{
    int r;
    int g;
    int b;

    color(int _r, int _g, int _b){
        r = _r;
        g = _g;
        b = _b;
    }
};

class GuiButton{
/* code can be much better but i didnt have much time to fix it and i had to make the program work */
public:
    int x, y, width, height, value;

    GuiButton(int _x, int _y, int _width, int _height, int _value){
        x = _x;
        y = _y;
        width = _width;
        height = _height;
        value = _value;
    }

    void DrawGuiButton(){
        string valueString = to_string(value);
        DrawRectangle(x,y,width,height,Color{45,110,189,255});
        if(value < 0){
            DrawText(valueString.c_str(), x+5, y+10, 40, Color{16,40,69,255});
        }else{
            DrawText(valueString.c_str(), x+20, y+10, 40, Color{16,40,69,255});
        }
    }

    void DrawGuiButtoncalculate(){
        DrawRectangle(x,y,width,height,Color{45,110,189,255});
        DrawRectangleLinesEx(Rectangle{x,y,width,height}, 6.0f, Color{21,52,89,255});
        DrawText("=", x+90, y+15, 55, Color{16,40,69,255});
    }

    void DrawGuiButtonSave(){
        DrawRectangle(x,y,width,height,Color{45,110,189,255});
        DrawRectangleLinesEx(Rectangle{x,y,width,height}, 6.0f, Color{21,52,89,255});
        DrawText("save", x+62, y+25, 30, Color{16,40,69,255});
    }

    void GuiPress(){
        Vector2 MousePos = GetMousePosition();
        if(MousePos.x >= x && MousePos.x <= x+width && MousePos.y >= y && MousePos.y <= y+height){
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) value -= 1;
            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) value += 1;
            clamp(value,-9,9);
        }
    }

    bool GuiCalculate(){
        Vector2 MousePos = GetMousePosition();
        if(MousePos.x >= x && MousePos.x <= x+width && MousePos.y >= y && MousePos.y <= y+height){
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                return true;
            }
        }
        return false;
    }
};

class GuiTemplate{
public:
    int x, y, width, height;

    //constructor ( just so i dont forget:) )
    GuiTemplate(int _x, int _y, int _width, int _height){
        x = _x;
        y = _y;
        width = _width;
        height = _height;
    }

    void DrawGuiTemplate(){
        DrawRectangle(x, y, width, 25, Color{10,10,10,255});
        DrawRectangle(x, y+25, width, height, Color{25,25,25,255});
        DrawText("Kernel",x+3,y+3,19,WHITE);
    }
};

int main() {

    Vector2 origin = {88,150};
    string Version;
    int ImgWidth;
    int ImgHeight;
    int MaxColor;
    const int ScreenWidth = 1450;
    const int ScreenHeight = 900;
    float maxscale = 850;
    float scale;

    //create the file object
    string FolderPath = "input/";
    string filename;
    ifstream File;
    ofstream FileOut;

    while (true) {

        system("clear");
        cout << R"(
  _____  _____  __  __   _  __                    _
 |  __ \|  __ \|  \/  | | |/ /                   | |
 | |__) | |__) | \  / | | ' / ___ _ __ _ __   ___| |
 |  ___/|  ___/| |\/| | |  < / _ \ '__| '_ \ / _ \ |
 | |    | |    | |  | | | . \  __/ |  | | | |  __/ |
 |_|    |_|    |_|  |_| |_|\_\___|_|  |_| |_|\___|_|
    )" << endl;

    cout << "\n";
    cout << ">>>File name is case sensitive<<<" <<endl;
    cout << ">>>     Must be version 3     <<<" <<endl;
    cout << "Enter the name of your file: ";
    cin >> filename;
    cout << "\n" <<endl;

    File.open(FolderPath+filename);
    if(!File.is_open()){

        File.clear();
        cout << "File failed to open!" <<endl;
        cout << "Press enter to continue";
        cin.ignore();
        cin.get();

    }else{
        break;
    }
}

    //gives the version image width, height, and max color
    File >> Version >> ImgWidth >> ImgHeight >> MaxColor;

    //create a 2d vector
    //asign each index with a color struct (r,g,b) of 0
    vector<vector<color>> image(ImgWidth, vector<color>(ImgHeight,color(0,0,0)));
    vector<vector<color>> imageBuffer(ImgWidth, vector<color>(ImgHeight,color(0,0,0)));

    for(int y = 0; y < ImgHeight; y++){
        for(int x = 0; x < ImgWidth; x++){
            int red, green, blue;
            File >> red >> green >> blue;
            image[x][y] = color(red,green,blue);
            //this can also work
            /*image[x][y].r = red;
            image[x][y].g = green;
            image[x][y].b = blue;*/
        }
    }

    //finds the ratio between image width and scale so that in the drawing part the image width is the same as scale to keep the image in bounds
    scale = ImgWidth/maxscale;

    vector<int> Kernel = {0,0,0,
                          0,0,0,
                          0,0,0};

    InitWindow(ScreenWidth, ScreenHeight, "PPM Kernel");
    SetWindowState(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);

    RenderTexture2D screen = LoadRenderTexture(ImgWidth,ImgHeight);
    BeginTextureMode(screen);
    ClearBackground(BLANK);
    for(int y = 0; y < ImgHeight; y++){
            for(int x = 0; x < ImgWidth; x++){
                int r = image[x][y].r;
                int g = image[x][y].g;
                int b = image[x][y].b;
                DrawRectangleV(Vector2{x,y}, {1,1}, Color{r,g,b,255});
            }
        }

    EndTextureMode();

    GuiTemplate test(50,100,300,500);
    GuiButton button1(origin.x, origin.y, 60, 60, 0);
    GuiButton button2(origin.x+60+20, origin.y, 60, 60, 0);
    GuiButton button3(origin.x+120+40, origin.y, 60, 60, 0);

    GuiButton button4(origin.x, origin.y+60+20, 60, 60, 0);
    GuiButton button5(origin.x+60+20, origin.y+60+20, 60, 60, 0);
    GuiButton button6(origin.x+120+40, origin.y+60+20, 60, 60, 0);

    GuiButton button7(origin.x, origin.y+120+40, 60, 60, 0);
    GuiButton button8(origin.x+60+20, origin.y+120+40, 60, 60, 0);
    GuiButton button9(origin.x+120+40, origin.y+120+40, 60, 60, 0);
    GuiButton calculateButton(origin.x+12,origin.y+250,200,80,0);
    GuiButton saveButton(origin.x+12,origin.y+350,200,80,0);

    while (!WindowShouldClose()) {

        if(calculateButton.GuiCalculate()){

            Kernel[0] = button1.value;
            Kernel[1] = button2.value;
            Kernel[2] = button3.value;
            Kernel[3] = button4.value;
            Kernel[4] = button5.value;
            Kernel[5] = button6.value;
            Kernel[6] = button7.value;
            Kernel[7] = button8.value;
            Kernel[8] = button9.value;

            int divisor = 0;
            for(int i = 0; i < Kernel.size(); i++){
                divisor += Kernel[i];
            }
            if(divisor == 0) divisor = 1; //no division by zero error

            BeginTextureMode(screen);
            ClearBackground(BLANK);
            for(int y = 0; y < ImgHeight; y++){
                for(int x = 0; x < ImgWidth; x++){
                    int KernelSumr = 0;
                    int KernelSumg = 0;
                    int KernelSumb = 0;
                    int KernelValuer = 0;
                    int KernelValueg = 0;
                    int KernelValueb = 0;


                    //top left, top, top right
                    if(x > 0 && y > 0){
                         KernelSumr += image[x-1][y-1].r * Kernel[0];
                         KernelSumg += image[x-1][y-1].g * Kernel[0];
                         KernelSumb += image[x-1][y-1].b * Kernel[0];
                    }
                    if(y > 0){
                        KernelSumr += image[x][y-1].r * Kernel[1];
                        KernelSumg += image[x][y-1].g * Kernel[1];
                        KernelSumb += image[x][y-1].b * Kernel[1];
                    }
                    if(x < ImgWidth-2 && y > 0){
                        KernelSumr += image[x+1][y-1].r * Kernel[2];
                        KernelSumg += image[x+1][y-1].g * Kernel[2];
                        KernelSumb += image[x+1][y-1].b * Kernel[2];
                    }

                    //left, center, right
                    if(x > 0){
                        KernelSumr += image[x-1][y].r * Kernel[3];
                        KernelSumg += image[x-1][y].g * Kernel[3];
                        KernelSumb += image[x-1][y].b * Kernel[3];
                    }

                    KernelSumr += image[x][y].r * Kernel[4];
                    KernelSumg += image[x][y].g * Kernel[4];
                    KernelSumb += image[x][y].b * Kernel[4];

                    if(x < ImgWidth-2){
                        KernelSumr += image[x+1][y].r * Kernel[5];
                        KernelSumg += image[x+1][y].g * Kernel[5];
                        KernelSumb += image[x+1][y].b * Kernel[5];
                    }

                    //bottom left, bottom, bottom right
                    if(x > 0 && y < ImgHeight-2){
                        KernelSumr += image[x-1][y+1].r * Kernel[6];
                        KernelSumg += image[x-1][y+1].g * Kernel[6];
                        KernelSumb += image[x-1][y+1].b * Kernel[6];
                    }
                    if(y < ImgHeight-2){
                        KernelSumr += image[x][y+1].r * Kernel[7];
                        KernelSumg += image[x][y+1].g * Kernel[7];
                        KernelSumb += image[x][y+1].b * Kernel[7];
                    }
                    if(x < ImgWidth-2 && y < ImgHeight-2){
                        KernelSumr += image[x+1][y+1].r * Kernel[8];
                        KernelSumg += image[x+1][y+1].g * Kernel[8];
                        KernelSumb += image[x+1][y+1].b * Kernel[8];
                    }

                    KernelValuer = KernelSumr/divisor;
                    KernelValueg = KernelSumg/divisor;
                    KernelValueb = KernelSumb/divisor;

                    clamp(KernelValuer, 0, 255);
                    clamp(KernelValueg, 0, 255);
                    clamp(KernelValueb, 0, 255);

                    imageBuffer[x][y].r = KernelValuer;
                    imageBuffer[x][y].g = KernelValueg;
                    imageBuffer[x][y].b = KernelValueb;

                    DrawRectangleV(Vector2{x,y}, Vector2{1,1}, Color{imageBuffer[x][y].r,imageBuffer[x][y].g,imageBuffer[x][y].b,255});
                }
            }
            EndTextureMode();
        }

        BeginDrawing();
        ClearBackground(Color{216,230,227});
        //y is set to negative because the texture is flipped so we draw it back in the correct orientation
        DrawTexturePro(screen.texture, Rectangle{0,0,ImgWidth,-ImgHeight}, Rectangle{400,100,ImgWidth/scale,ImgHeight/scale}, Vector2{0,0}, 0.0f, WHITE);

        //draw the gui buttons
        test.DrawGuiTemplate();

        button1.DrawGuiButton();
        button1.GuiPress();

        button2.DrawGuiButton();
        button2.GuiPress();

        button3.DrawGuiButton();
        button3.GuiPress();

        button4.DrawGuiButton();
        button4.GuiPress();

        button5.DrawGuiButton();
        button5.GuiPress();

        button6.DrawGuiButton();
        button6.GuiPress();

        button7.DrawGuiButton();
        button7.GuiPress();

        button8.DrawGuiButton();
        button8.GuiPress();

        button9.DrawGuiButton();
        button9.GuiPress();

        calculateButton.DrawGuiButtoncalculate();
        saveButton.DrawGuiButtonSave();

        EndDrawing();

        if(saveButton.GuiCalculate()){
            FileOut.open("output/output.ppm");
            FileOut << Version << "\n" << ImgWidth << " " << ImgHeight << "\n" << MaxColor << "\n";
            for(int y = 0; y < ImgHeight; y++){
                for(int x = 0; x < ImgWidth; x++){
                    FileOut << imageBuffer[x][y].r << " " << imageBuffer[x][y].g << " " << imageBuffer[x][y].b << "\n";
                }
            }
            FileOut.close();
        }
    }

    CloseWindow();
    return 0;
}
