#include "raylib.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plag_main.h"


// THE JSON FILE CONFIGURATION IS DESGINED TO RUN main.c only and this is only for submission purposes



bool flag=false;

typedef enum {
    SCREEN_WELCOME,
    SCREEN_MAIN,            //adding all the screens
    SCREEN_ABOUT,
    SCREEN_DETECT_PLAGIARISM 
} ScreenState;

int main(void)
{   double score = plag_check();
    ScreenState currentScreen = SCREEN_WELCOME;

    const int screenWidth = 1920;
    const int screenHeight = 1200;
    char file1Path[256] = "";
    char file2Path[256] = "";
    bool file1Selected = false;
    bool file2Selected = false;
    bool showCongratMessage = false;
    const char *selectedCongratPhrase = NULL;
    InitWindow(screenWidth, screenHeight, "Introducing DejaCode");
    InitAudioDevice();
    ToggleFullscreen();
    SetTargetFPS(60);

    Texture2D bg = LoadTexture("intro_invert.png");
    Texture2D fullImage = LoadTexture("team.jpeg");
    bool showImageFullScreen = false;

    Sound buttonClick = LoadSound("click.wav");
    Sound Error = LoadSound("Error.wav");
    Sound intro = LoadSound("intro.mp3");
    const int fontSize = 30;
    Font customFont = LoadFontEx("Rajdhani-Semibold.ttf", fontSize, 0, 250);
    Font titleFont = LoadFontEx("Rajdhani-Semibold.ttf", 70, 0, 250);
    Font introFont = LoadFontEx("Rajdhani-Semibold.ttf", 36, 0, 250);
    SetTextureFilter(customFont.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(titleFont.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(introFont.texture, TEXTURE_FILTER_BILINEAR);

    if (introFont.texture.id == 0) introFont = GetFontDefault();
    if (customFont.texture.id == 0) customFont = GetFontDefault();
    if (titleFont.texture.id == 0) titleFont = GetFontDefault();

    // Main screen colors
    // Change the Detect Plagiarism button colors to #b84b0d
Color detectDefault = (Color){ 0xB8, 0x4B, 0x0D, 0xFF };  // New default color
Color detectHover = (Color){ 0xD0, 0x6A, 0x2A, 0xFF };   // Hover color for a lighter shade

    Color detectText = WHITE;

    Color aboutDefault = (Color){ 0xFC, 0xA3, 0x11, 0xFF };
    Color aboutHover = (Color){ 0xFF, 0xB3, 0x47, 0xFF };
    Color aboutTextColor = WHITE;

    Color aboutBG = (Color){ 149, 150, 157, 255 };
    Color backBtnDefault = (Color){ 66, 129, 164, 255 };
    Color backBtnHover = (Color){ 78, 145, 185, 255 };
    Color backTextColor = WHITE;

    const char *introPhrases[5] = {
        "Unlock the truth behind your text.",
        "Dive in and test your originality.",
        "Test your text and stand out!",
        "Time to reveal the authenticity of your text.",
        "Start your quest for originality now!"
    };
    const char *congratPhrases[6] = {
        "Achievement Unlocked!,First F grade ",
        "Incredible! Your work has earned the legendary F grade!",
        "Congratulations on receiving the inaugural F grade!",
        "Cheers! That's the first F grade, and it's all yours!",
        "Well done, you're the pioneer of the F grade club!",
        "Hooray! The first F grade in the course goes to you!"
    };

    srand((unsigned int)time(NULL));
    const char *introPhrase = introPhrases[rand() % 5];

    const char *compareText = "Detect Plagiarism";
    const char *aboutText = "About Us";
    const char *backText = "Back to Menu";
    const char *titleText = "DejaCode";

    // Welcome screen variables
    float welcomeAlpha = 0.0f;
    float alphaSpeed = 0.015f;
    int welcomeFrames = 0;
    bool fadeInComplete = false;
    bool fadeOutComplete = false;


    while (!WindowShouldClose())
    {
        Vector2 mousePos = GetMousePosition();

        BeginDrawing();
        
        if (showImageFullScreen)
        {
            ClearBackground(BLACK);
            DrawTexturePro(fullImage,
                (Rectangle){ 0, 0, fullImage.width, fullImage.height },
                (Rectangle){ 0, 0, screenWidth, screenHeight },
                (Vector2){ 0, 0 },
                0.0f,
                WHITE);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showImageFullScreen = false;
            }

            EndDrawing();
            continue;
        }

        if (currentScreen == SCREEN_WELCOME)
{
    ClearBackground(BLACK);

    if (!fadeInComplete)
    {   PlaySound(intro);
        welcomeAlpha += alphaSpeed;
        if (welcomeAlpha >= 1.0f)
        {
            welcomeAlpha = 1.0f;
            fadeInComplete = true;
        }
    }
    else if (!fadeOutComplete)
    {   
        welcomeFrames++;

        if (welcomeFrames >= 90) // Wait for ~1.5 seconds before fading out
        {
            welcomeAlpha -= alphaSpeed;
            if (welcomeAlpha <= 0.0f)
            {
                welcomeAlpha = 0.0f;
                fadeOutComplete = true;
                currentScreen = SCREEN_MAIN;
            }
        }
    }

    Color fadeColor = (Color){ 255, 255, 255, (unsigned char)(welcomeAlpha * 255) };
    Vector2 textSize = MeasureTextEx(titleFont, "Introducing DejaCode", 60, 1.0f);
    
    DrawTextEx(titleFont, "Introducing DejaCode",
        (Vector2){ screenWidth / 2.0f - textSize.x / 2, screenHeight / 2.0f - 50 },
        60, 1.0f, fadeColor);
}

        else if (currentScreen == SCREEN_MAIN)
        {
            ClearBackground(RAYWHITE);
            
            float centerX = (float)(screenWidth - 400) / 2.0f;
            float compareBtnY = (float)(screenHeight / 2 - 80 - 20 + 80);
            float aboutBtnY = (float)(screenHeight / 2 + 20 + 80);

            Rectangle compareBtn = { centerX, compareBtnY, 400, 80 };
            Rectangle aboutBtn = { centerX, aboutBtnY, 400, 80 };

            bool hoverCompare = CheckCollisionPointRec(mousePos, compareBtn);
            bool hoverAbout = CheckCollisionPointRec(mousePos, aboutBtn);
            bool clickCompare = hoverCompare && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
            bool clickAbout = hoverAbout && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

            if (clickCompare || clickAbout) PlaySound(buttonClick);
            if (clickAbout) currentScreen = SCREEN_ABOUT;
            if (clickCompare) currentScreen = SCREEN_DETECT_PLAGIARISM;

            Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
            float scaleX = (float)screenWidth / bg.width;
            float scaleY = (float)screenHeight / bg.height;
            float scaleFactor = (scaleX > scaleY) ? scaleX : scaleY;
            float dstW = bg.width * scaleFactor;
            float dstH = bg.height * scaleFactor;
            Rectangle dst = { screenWidth / 2.0f, screenHeight / 2.0f, dstW, dstH };
            Vector2 origin = { dstW / 2.0f, dstH / 2.0f };
            DrawTexturePro(bg, src, dst, origin, 0.0f, WHITE);

            Vector2 titleSize = MeasureTextEx(titleFont, titleText, 70, 1.0f);
            float titlePosX = screenWidth / 2.0f - titleSize.x / 2.0f;
            DrawTextEx(titleFont, titleText, (Vector2){ titlePosX, 150.0f }, 70, 1.0f, WHITE);

            Vector2 phraseSize = MeasureTextEx(introFont, introPhrase, 36, 1.0f);
            float phrasePosX = screenWidth / 2.0f - phraseSize.x / 2.0f;
            float phrasePosY = screenHeight / 2.0f - 160;
            DrawTextEx(introFont, introPhrase, (Vector2){ phrasePosX, phrasePosY }, 36, 1.0f, WHITE);

            Color compareColor = hoverCompare ? detectHover : detectDefault;
            DrawRectangleRounded(compareBtn, 0.3f, 10, compareColor);
            Vector2 compareTextSize = MeasureTextEx(customFont, compareText, fontSize, 1.0f);
            DrawTextEx(customFont, compareText,
                (Vector2){ compareBtn.x + (400 - compareTextSize.x) / 2, compareBtn.y + (80 - compareTextSize.y) / 2 },
                fontSize, 1.0f, detectText);

            Color aboutColor = hoverAbout ? aboutHover : aboutDefault;
            DrawRectangleRounded(aboutBtn, 0.3f, 10, aboutColor);
            Vector2 aboutTextSize = MeasureTextEx(customFont, aboutText, fontSize, 1.0f);
            DrawTextEx(customFont, aboutText,
                (Vector2){ aboutBtn.x + (400 - aboutTextSize.x) / 2, aboutBtn.y + (80 - aboutTextSize.y) / 2 },
                fontSize, 1.0f, aboutTextColor);
        }
        else if (currentScreen == SCREEN_ABOUT)
        {
            ClearBackground(aboutBG);

            DrawTextEx(titleFont, "About Our Team", (Vector2){ 100, 100 }, 60, 1.0f, BLACK);
            DrawTextEx(customFont, "Team Members:", (Vector2){ 100, 200 }, 42, 1.0f, DARKGRAY);

            DrawTextEx(customFont, "- Hrujul Mendhe:    Ideation and Part of backend",     (Vector2){ 120, 260 }, 34, 1.0f, BLACK);
            DrawTextEx(customFont, "- Pragyan Das:      Prototyping of backend, integration and code cleaning",   (Vector2){ 120, 360 }, 34, 1.0f, BLACK);
            DrawTextEx(customFont, "- Priyam Patel:     Frontend designing of plagiarism page and file handling system",    (Vector2){ 120, 460 }, 34, 1.0f, BLACK);
            DrawTextEx(customFont, "- Ved Saraiya:      Frontend designing of about us page and coding and beautification of UI",     (Vector2){ 120, 560 }, 34, 1.0f, BLACK);
            DrawTextEx(customFont, "- Omprakash:        Frontend designing of home page and code cleaning",       (Vector2){ 120, 660 }, 34, 1.0f, BLACK);

            Rectangle backBtn = { screenWidth - 320, screenHeight - 140, 250, 60 };
            Rectangle viewImageBtn = { screenWidth - 600, screenHeight - 140, 250, 60 };

            bool hoverBack = CheckCollisionPointRec(mousePos, backBtn);
            bool hoverViewImage = CheckCollisionPointRec(mousePos, viewImageBtn);
            bool clickBack = hoverBack && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
            bool clickViewImage = hoverViewImage && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

            if (clickBack) {
                PlaySound(buttonClick);
                currentScreen = SCREEN_MAIN;
            }

            if (clickViewImage) {
                PlaySound(buttonClick);
                showImageFullScreen = true;
            }

            DrawRectangleRounded(backBtn, 0.3f, 10, hoverBack ? backBtnHover : backBtnDefault);
            Vector2 backSize = MeasureTextEx(customFont, backText, fontSize, 1.0f);
            DrawTextEx(customFont, backText,
                (Vector2){ backBtn.x + (250 - backSize.x) / 2, backBtn.y + (60 - backSize.y) / 2 },
                fontSize, 1.0f, backTextColor);

            DrawRectangleRounded(viewImageBtn, 0.3f, 10, hoverViewImage ? backBtnHover : backBtnDefault);
            Vector2 imageSize = MeasureTextEx(customFont, "View Image", fontSize, 1.0f);
            DrawTextEx(customFont, "View Image",
                (Vector2){ viewImageBtn.x + (250 - imageSize.x) / 2, viewImageBtn.y + (60 - imageSize.y) / 2 },
                fontSize, 1.0f, backTextColor);
        }
        // Replace the plagiarism detection screen section with this dark-themed version
else if (currentScreen == SCREEN_DETECT_PLAGIARISM)
{
    // Dark theme background
    Color darkBG = (Color){ 25, 30, 40, 255 };
    Color lightText = (Color){ 235, 235, 235, 255 };
    Color secondaryText = (Color){ 180, 190, 200, 255 };
    Color resultBoxBG = (Color){ 45, 50, 60, 255 };
    
    ClearBackground(darkBG);
    
    // Draw title with light text
    Vector2 titleSize = MeasureTextEx(titleFont, "Plagiarism Detection", 60, 1.0f);
    float titlePosX = screenWidth / 2.0f - titleSize.x / 2.0f;
    DrawTextEx(titleFont, "Plagiarism Detection", (Vector2){ titlePosX, 100.0f }, 60, 1.0f, lightText);
    
    // Draw instructions with secondary text color
    Vector2 instructSize = MeasureTextEx(customFont, "Select files to compare for plagiarism:",75, 1.0f);
    float instructPosX = screenWidth / 2.0f - instructSize.x / 2.0f;
    DrawTextEx(customFont, "Select files to compare for plagiarism(Paste the code in input folder in txt files)", 
        (Vector2){ instructPosX, 200.0f }, 36, 1.0f, secondaryText);
    
    // File selection buttons - keep existing colors for contrast
    Rectangle fileBtn1 = { screenWidth / 2.0f - 400, 300, 350, 80 };
    Rectangle fileBtn2 = { screenWidth / 2.0f + 50, 300, 350, 80 };
    
    bool hoverFile1 = CheckCollisionPointRec(mousePos, fileBtn1);
    bool hoverFile2 = CheckCollisionPointRec(mousePos, fileBtn2);
    bool clickFile1 = hoverFile1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool clickFile2 = hoverFile2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    // Handle file 1 selection
    if (clickFile1) {
        PlaySound(buttonClick);
        
        // Store current state to a file
        FILE *tempFile = fopen("temp_file_selection", "w");
        if (tempFile) {
            fprintf(tempFile, "1"); // Indicates selecting first file
            fclose(tempFile);
        }
        
        // Simulate file selection
        strcpy(file1Path, "INPUT_FOLDER/first.txt");
        file1Selected = true;
    }
    
    // Handle file 2 selection
    if (clickFile2) {
        PlaySound(buttonClick);
        
        // Store current state
        FILE *tempFile = fopen("temp_file_selection", "w");
        if (tempFile) {
            fprintf(tempFile, "2"); // Indicates selecting second file
            fclose(tempFile);
        }
        
        // Simulate file selection for demo
        strcpy(file2Path, "INPUT_FOLDER/second.txt");
        file2Selected = true;
    }
    
    // Draw file 1 button
    DrawRectangleRounded(fileBtn1, 0.3f, 10, hoverFile1 ? aboutHover : aboutDefault);
    
    char file1ButtonText[40] = "Select First File";
    if (file1Selected) {
        // Show shortened filename
        const char *filename = GetFileName(file1Path);
        sprintf(file1ButtonText, "File 1: %s", filename);
    }
    
    Vector2 file1Size = MeasureTextEx(customFont, file1ButtonText, fontSize, 1.0f);
    DrawTextEx(customFont, file1ButtonText,
        (Vector2){ fileBtn1.x + (350 - file1Size.x) / 2, fileBtn1.y + (80 - file1Size.y) / 2 },
        fontSize, 1.0f, WHITE);
    
    // Draw file 2 button
    DrawRectangleRounded(fileBtn2, 0.3f, 10, hoverFile2 ? aboutHover : aboutDefault);
    
    char file2ButtonText[40] = "Select Second File";
    if (file2Selected) {
        // Show shortened filename
        const char *filename = GetFileName(file2Path);
        sprintf(file2ButtonText, "File 2: %s", filename);
    }
    
    Vector2 file2Size = MeasureTextEx(customFont, file2ButtonText, fontSize, 1.0f);
    DrawTextEx(customFont, file2ButtonText,
        (Vector2){ fileBtn2.x + (350 - file2Size.x) / 2, fileBtn2.y + (80 - file2Size.y) / 2 },
        fontSize, 1.0f, WHITE);
    
    // Compare button - only active if both files selected
    Rectangle compareBtn = { screenWidth / 2.0f - 175, 450, 350, 80 };
    bool canCompare = file1Selected && file2Selected;
    bool hoverCompareBtn = canCompare && CheckCollisionPointRec(mousePos, compareBtn);
    bool clickCompareBtn = hoverCompareBtn && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    // Use a dimmed color if files aren't selected yet
    Color compareBtnColor = canCompare ? (hoverCompareBtn ? detectHover : detectDefault) : (Color){ 70, 70, 70, 255 };
    
    DrawRectangleRounded(compareBtn, 0.3f, 10, compareBtnColor);
    Vector2 compareBtnSize = MeasureTextEx(customFont, "Compare Files", fontSize, 1.0f);
    DrawTextEx(customFont, "Compare Files",
        (Vector2){ compareBtn.x + (350 - compareBtnSize.x) / 2, compareBtn.y + (80 - compareBtnSize.y) / 2 },
        fontSize, 1.0f, WHITE);
    
    // Handle compare button click
    if (clickCompareBtn) {
        if (score > 90) {
            PlaySound(Error);
            // Select a random congratulatory message
            selectedCongratPhrase = congratPhrases[rand() % 5];
            showCongratMessage = true;
        }
        else {
            PlaySound(buttonClick);
            showCongratMessage = false;
        }
        flag = true;
    }
    
    // Results display area with darker background
    DrawRectangleRounded((Rectangle){ screenWidth / 2.0f - 500, 580, 1000, 400 }, 0.1f, 10, resultBoxBG);

    // Show appropriate message based on selection state
    const char* resultText;
    if (!file1Selected || !file2Selected) {
        resultText = "Please select two files to compare";
    } else if (!flag) {
        resultText = "Press 'Compare Files' to analyze for plagiarism";
    } else {
        char num_string[40];
        
        sprintf(num_string, "AND THE SIMILARITY SCORE IS %0.2f", score);
        int length = strlen(num_string);
        num_string[length]= '%';
        num_string[length+1] = '\0';
        resultText = num_string;
        
        // Draw the score with light text
        Vector2 resultSize = MeasureTextEx(customFont, resultText, fontSize, 1.0f);
        DrawTextEx(customFont, resultText,
            (Vector2){ screenWidth / 2.0f - resultSize.x / 2, 700 }, fontSize, 1.0f, lightText);
        
        // Draw congratulatory message if applicable
        if (showCongratMessage && selectedCongratPhrase != NULL) {
            Vector2 congratSize = MeasureTextEx(customFont, selectedCongratPhrase, fontSize, 1.0f);
            DrawTextEx(customFont, selectedCongratPhrase,
                (Vector2){ screenWidth / 2.0f - congratSize.x / 2, 760 }, fontSize, 1.0f, RED);
        }
        
        goto SkipResultText; // Skip redundant text drawing
    }

    Vector2 resultSize = MeasureTextEx(customFont, resultText, fontSize, 1.0f);
    DrawTextEx(customFont, resultText,
        (Vector2){ screenWidth / 2.0f - resultSize.x / 2, 760 }, fontSize, 1.0f, lightText);

SkipResultText:
        
    // Back button - keep existing colors for consistency
    Rectangle backBtn = { screenWidth - 320, screenHeight - 140, 250, 60 };
    bool hoverBack = CheckCollisionPointRec(mousePos, backBtn);
    bool clickBack = hoverBack && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    if (clickBack) {
        PlaySound(buttonClick);
        currentScreen = SCREEN_MAIN;
    }
    
    DrawRectangleRounded(backBtn, 0.3f, 10, hoverBack ? backBtnHover : backBtnDefault);
    Vector2 backSize = MeasureTextEx(customFont, backText, fontSize, 1.0f);
    DrawTextEx(customFont, backText,
        (Vector2){ backBtn.x + (250 - backSize.x) / 2, backBtn.y + (60 - backSize.y) / 2 },
        fontSize, 1.0f, backTextColor);
}

// Add this helper function at the end of your file
// const char* GetFileName(const char* path) 
// {
//     const char* filename = strrchr(path, '/');
//     if (!filename) filename = strrchr(path, '\\');
    
//     if (filename) {
//         return filename + 1;
//     }
    
//     return path;  // If no path separator found, return the original string
// }

        EndDrawing();
    }

    UnloadTexture(bg);
    UnloadTexture(fullImage);
    UnloadFont(customFont);
    UnloadFont(introFont);
    UnloadFont(titleFont);
    UnloadSound(buttonClick);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
