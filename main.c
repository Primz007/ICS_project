#include "raylib.h"   //For frontend
#include <time.h>     //For displaying random phrases
#include <stdlib.h>   
#include <string.h>   
#include <stdio.h>    
#include "plag_main.h" //Plag checker function and function for top 5 most words used

// Flag to track if comparison has been performed
bool flag=false;

// Enum to define different screens 
typedef enum {      
    SCREEN_WELCOME,            // Initial welcome screen
    SCREEN_MAIN,              // Main menu screen
    SCREEN_ABOUT,             // About us screen
    SCREEN_DETECT_PLAGIARISM  // Plagiarism detection screen
} ScreenState;


int main(void)
{   
    // Get plagiarism score from the checker function
    double score = plag_check();

    // Current screen state initialized to welcome screen
    ScreenState currentScreen = SCREEN_WELCOME;

    // Window dimensions
    int screenWidth = 1920;   
    int screenHeight = 1200;
    
    // File paths
    char file1Path[256] = "first.txt"; //accesing file location in same directory in input folder
    char file2Path[256] = "second.txt";
    bool file1Selected = false;         
    bool file2Selected = false;
    bool showCongratMessage = false;
    char *selectedCongratPhrase = NULL;

    // Initialize window and audio
    InitWindow(screenWidth, screenHeight, "Introducing DejaCode");
    InitAudioDevice();
    ToggleFullscreen();
    SetTargetFPS(60);

    // Load textures
    Texture2D bg = LoadTexture("intro_invert.png");
    Texture2D fullImage = LoadTexture("team.jpeg");
    bool showImageFullScreen = false;

    // Load sound effects
    Sound buttonClick = LoadSound("click.wav");
    Sound Error = LoadSound("Error.wav");
    Sound intro = LoadSound("intro.mp3");

    // Load and configure fonts
    int fontSize = 30;
    int titleFontSize = 70;
    int introFontSize = 36;
    Font customFont = LoadFontEx("Rajdhani-Semibold.ttf", fontSize, 0, 250);
    Font titleFont = LoadFontEx("Rajdhani-Semibold.ttf", titleFontSize, 0, 250);
    Font introFont = LoadFontEx("Rajdhani-Semibold.ttf", introFontSize, 0, 250);
    SetTextureFilter(customFont.texture, TEXTURE_FILTER_BILINEAR); // Set texture filter for custom font
    SetTextureFilter(titleFont.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(introFont.texture, TEXTURE_FILTER_BILINEAR);

    
    
    // Color definitions for UI elements
    Color detectDefault = (Color){ 0xB8, 0x4B, 0x0D, 0xFF };  // Default button color
    Color detectHover = (Color){ 0xD0, 0x6A, 0x2A, 0xFF };   // Hover button color
    Color detectText = WHITE;
    Color aboutDefault = (Color){ 0xFC, 0xA3, 0x11, 0xFF };
    Color aboutHover = (Color){ 0xFF, 0xB3, 0x47, 0xFF };
    Color aboutTextColor = WHITE;
    Color aboutBG = (Color){ 149, 150, 157, 255 };
    Color backBtnDefault = (Color){ 66, 129, 164, 255 };
    Color backBtnHover = (Color){ 78, 145, 185, 255 };
    Color backTextColor = WHITE;
    // Arrays of random phrases for UI
    char *introPhrases[5] = {
        "Unlock the truth behind your text.",
        "Dive in and test your originality.",
        "Test your text and stand out!",
        "Time to reveal the authenticity of your text.",
        "Start your quest for originality now!"
    };
    char *congratPhrases[6] = {
        "Achievement Unlocked!,First F grade ",
        "Incredible! Your work has earned the legendary F grade!",
        "Congratulations on receiving the inaugural F grade!",
        "Cheers! That's the first F grade, and it's all yours!",
        "Well done, you're the pioneer of the F grade club!",
        "Hooray! The first F grade in the course goes to you!"
    };

    // Initialize random seed and select random intro phrase
    srand((unsigned int)time(NULL));
    char *introPhrase = introPhrases[rand() % 5];

    // UI text constants
    char *compareText = "Detect Plagiarism";
    char *aboutText = "About Us";
    char *backText = "Back to Menu";
    char *titleText = "DejaCode";

    // Welcome screen animation variables
    float welcomeAlpha = 0.0f;
    float alphaSpeed = 0.015f;
    int welcomeFrames = 0;
    bool fadeInComplete = false;
    bool fadeOutComplete = false;

    // Main game loop
    while (!WindowShouldClose())
    {
        // Get current mouse position
        Vector2 mousePos = GetMousePosition();

        // Start drawing frame
        BeginDrawing();
        
        // Handle full screen image view
        if (showImageFullScreen)
        {
            ClearBackground(BLACK);
            // Draw the team image in full screen
            Rectangle src = { 0, 0, fullImage.width, fullImage.height };
            Rectangle dest = { 0, 0, screenWidth, screenHeight };
            Vector2 origin = { 0, 0 };

             DrawTexturePro(fullImage, src, dest, origin, 0.0f, WHITE);

            // Exit full screen on mouse click
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showImageFullScreen = false;
            }

            EndDrawing();
            continue;
        }

        // Welcome screen handling
        if (currentScreen == SCREEN_WELCOME)
        {
            ClearBackground(BLACK);

            // Handle fade-in animation
            if (!fadeInComplete)
            {   
                PlaySound(intro);
                welcomeAlpha += alphaSpeed;
                if (welcomeAlpha >= 1.0f)
                {
                    welcomeAlpha = 1.0f;
                    fadeInComplete = true;
                }
            }
            // Handle fade-out animation
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

            // Draw welcome text with fade effect
            Color fadeColor = (Color){ 255, 255, 255, (unsigned char)(welcomeAlpha * 255) };
            Vector2 textSize = MeasureTextEx(titleFont, "Introducing DejaCode", 60, 1.0f);
            
            DrawTextEx(titleFont, "Introducing DejaCode",
                (Vector2){ screenWidth / 2.0f - textSize.x / 2, screenHeight / 2.0f - 50 },
                60, 1.0f, fadeColor);
        }

        // Main screen handling
        else if (currentScreen == SCREEN_MAIN)
        {
            ClearBackground(RAYWHITE);
            
            // Calculate button positions
            float centerX = (float)(screenWidth - 400) / 2.0f;
            float compareBtnY = (float)(screenHeight / 2 - 80 - 20 + 80);
            float aboutBtnY = (float)(screenHeight / 2 + 20 + 80);

            // Define button rectangles
            Rectangle compareBtn = { centerX, compareBtnY, 400, 80 };
            Rectangle aboutBtn = { centerX, aboutBtnY, 400, 80 };

            // Handle button hover and click states
            bool hoverCompare = CheckCollisionPointRec(mousePos, compareBtn);
            bool hoverAbout = CheckCollisionPointRec(mousePos, aboutBtn);
            bool clickCompare = hoverCompare && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
            bool clickAbout = hoverAbout && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

            // Handle button clicks and screen transitions
            if (clickCompare || clickAbout) PlaySound(buttonClick);
            if (clickAbout) currentScreen = SCREEN_ABOUT;
            if (clickCompare) currentScreen = SCREEN_DETECT_PLAGIARISM;

            // Draw background image
            Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
            float scaleX = (float)screenWidth / bg.width;
            float scaleY = (float)screenHeight / bg.height;
            float scaleFactor = (scaleX > scaleY) ? scaleX : scaleY;
            float dstW = bg.width * scaleFactor;
            float dstH = bg.height * scaleFactor;
            Rectangle dst = { screenWidth / 2.0f, screenHeight / 2.0f, dstW, dstH };
            Vector2 origin = { dstW / 2.0f, dstH / 2.0f };
            DrawTexturePro(bg, src, dst, origin, 0.0f, WHITE);

            // Draw title and intro phrase
            Vector2 titleSize = MeasureTextEx(titleFont, titleText, 70, 1.0f);
            float titlePosX = screenWidth / 2.0f - titleSize.x / 2.0f;
            DrawTextEx(titleFont, titleText, (Vector2){ titlePosX, 150.0f }, 70, 1.0f, WHITE);

            Vector2 phraseSize = MeasureTextEx(introFont, introPhrase, 36, 1.0f);
            float phrasePosX = screenWidth / 2.0f - phraseSize.x / 2.0f;
            float phrasePosY = screenHeight / 2.0f - 160;
            DrawTextEx(introFont, introPhrase, (Vector2){ phrasePosX, phrasePosY }, 36, 1.0f, WHITE);

            // Draw compare button
            Color compareColor = hoverCompare ? detectHover : detectDefault;
            DrawRectangleRounded(compareBtn, 0.3f, 10, compareColor);
            Vector2 compareTextSize = MeasureTextEx(customFont, compareText, fontSize, 1.0f);
            DrawTextEx(customFont, compareText,
                (Vector2){ compareBtn.x + (400 - compareTextSize.x) / 2, compareBtn.y + (80 - compareTextSize.y) / 2 },
                fontSize, 1.0f, detectText);

            // Draw about button
            Color aboutColor = hoverAbout ? aboutHover : aboutDefault;
            DrawRectangleRounded(aboutBtn, 0.3f, 10, aboutColor);
            Vector2 aboutTextSize = MeasureTextEx(customFont, aboutText, fontSize, 1.0f);
            DrawTextEx(customFont, aboutText,
                (Vector2){ aboutBtn.x + (400 - aboutTextSize.x) / 2, aboutBtn.y + (80 - aboutTextSize.y) / 2 },
                fontSize, 1.0f, aboutTextColor);
        }
        // About screen handling
        else if (currentScreen == SCREEN_ABOUT)
        {
            ClearBackground(aboutBG);

            // Draw about screen content
            DrawTextEx(titleFont, "About Our Team", (Vector2){ 100, 100 }, 60, 1.0f, BLACK);
            DrawTextEx(customFont, "Team Members:", (Vector2){ 100, 200 }, 42, 1.0f, DARKGRAY);

            // Draw team member information
            DrawTextEx(customFont, "- Hrujul Mendhe:    Ideation and Part of backend",     (Vector2){ 120, 260 }, 34, 1.0f, BLACK);
            DrawTextEx(customFont, "- Pragyan Das:      Prototyping of backend, integration and code cleaning",   (Vector2){ 120, 360 }, 34, 1.0f, BLACK);
            DrawTextEx(customFont, "- Priyam Patel:     Frontend designing of plagiarism page and file handling system",    (Vector2){ 120, 460 }, 34, 1.0f, BLACK);
            DrawTextEx(customFont, "- Ved Saraiya:      Frontend designing of about us page and coding and beautification of UI",     (Vector2){ 120, 560 }, 34, 1.0f, BLACK);
            DrawTextEx(customFont, "- Omprakash:        Frontend designing of home page and code cleaning",       (Vector2){ 120, 660 }, 34, 1.0f, BLACK);

            // Define and handle back and view image buttons
            Rectangle backBtn = { screenWidth - 320, screenHeight - 140, 250, 60 };
            Rectangle viewImageBtn = { screenWidth - 600, screenHeight - 140, 250, 60 };

            bool hoverBack = CheckCollisionPointRec(mousePos, backBtn);
            bool hoverViewImage = CheckCollisionPointRec(mousePos, viewImageBtn);
            bool clickBack = hoverBack && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
            bool clickViewImage = hoverViewImage && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

            // Handle button clicks
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
    // Define colors for dark theme
    Color darkBG = (Color){ 25, 30, 40, 255 };          // Dark background color
    Color lightText = (Color){ 235, 235, 235, 255 };    // Light text color for contrast
    Color secondaryText = (Color){ 180, 190, 200, 255 }; // Secondary text color
    Color resultBoxBG = (Color){ 45, 50, 60, 255 };     // Background color for result box
    
    ClearBackground(darkBG);
    
    // Draw main title at the top
    Vector2 titleSize = MeasureTextEx(titleFont, "Plagiarism Detection", 60, 1.0f);
    float titlePosX = screenWidth / 2.0f - titleSize.x / 2.0f;
    DrawTextEx(titleFont, "Plagiarism Detection", (Vector2){ titlePosX, 100.0f }, 60, 1.0f, lightText);
    
    // Draw instructions for file selection
    Vector2 instructSize = MeasureTextEx(customFont, "Select files to compare for plagiarism:",75, 1.0f);
    float instructPosX = screenWidth / 2.0f - instructSize.x / 2.0f;
    DrawTextEx(customFont, "Select files to compare for plagiarism(Paste the code in input folder in txt files)", 
        (Vector2){ instructPosX, 200.0f }, 36, 1.0f, secondaryText);
    
    // Define file selection button positions and sizes
    Rectangle fileBtn1 = { screenWidth / 2.0f - 400, 300, 350, 80 };
    Rectangle fileBtn2 = { screenWidth / 2.0f + 50, 300, 350, 80 };
    
    // Check for mouse hover and click on file buttons
    bool hoverFile1 = CheckCollisionPointRec(mousePos, fileBtn1);
    bool hoverFile2 = CheckCollisionPointRec(mousePos, fileBtn2);
    bool clickFile1 = hoverFile1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool clickFile2 = hoverFile2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    // Handle first file selection
    if (clickFile1) {
        PlaySound(buttonClick);
        
        file1Selected = true;
    }
    
    // Handle second file selection
    if (clickFile2) {
        PlaySound(buttonClick);
     
        file2Selected = true;
    }
    
    // Draw first file selection button
    DrawRectangleRounded(fileBtn1, 0.3f, 10, hoverFile1 ? aboutHover : aboutDefault);
    
    // Update button text based on file selection
    char file1ButtonText[40] = "Select First File";
    if (file1Selected) {
         char *filename = GetFileName(file1Path);
        sprintf(file1ButtonText, "File 1: %s", filename);
    }
    
    // Center and draw first file button text
    Vector2 file1Size = MeasureTextEx(customFont, file1ButtonText, fontSize, 1.0f);
    DrawTextEx(customFont, file1ButtonText,
        (Vector2){ fileBtn1.x + (350 - file1Size.x) / 2, fileBtn1.y + (80 - file1Size.y) / 2 },
        fontSize, 1.0f, WHITE);
    
    // Draw second file selection button
    DrawRectangleRounded(fileBtn2, 0.3f, 10, hoverFile2 ? aboutHover : aboutDefault);
    
    // Update button text based on file selection
    char file2ButtonText[40] = "Select Second File";
    if (file2Selected) {
        const char *filename = GetFileName(file2Path);
        sprintf(file2ButtonText, "File 2: %s", filename);
    }
    
    // Center and draw second file button text
    Vector2 file2Size = MeasureTextEx(customFont, file2ButtonText, fontSize, 1.0f);
    DrawTextEx(customFont, file2ButtonText,
        (Vector2){ fileBtn2.x + (350 - file2Size.x) / 2, fileBtn2.y + (80 - file2Size.y) / 2 },
        fontSize, 1.0f, WHITE);
    
    // Compare button setup
    Rectangle compareBtn = { screenWidth / 2.0f - 175, 450, 350, 80 };
    bool canCompare = file1Selected && file2Selected;
    bool hoverCompareBtn = canCompare && CheckCollisionPointRec(mousePos, compareBtn);
    bool clickCompareBtn = hoverCompareBtn && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    // Set compare button color based on whether comparison is possible
    Color compareBtnColor = canCompare ? (hoverCompareBtn ? detectHover : detectDefault) : (Color){ 70, 70, 70, 255 };
    
    // Draw compare button
    DrawRectangleRounded(compareBtn, 0.3f, 10, compareBtnColor);
    Vector2 compareBtnSize = MeasureTextEx(customFont, "Compare Files", fontSize, 1.0f);
    DrawTextEx(customFont, "Compare Files",
        (Vector2){ compareBtn.x + (350 - compareBtnSize.x) / 2, compareBtn.y + (80 - compareBtnSize.y) / 2 },
        fontSize, 1.0f, WHITE);
    
    // Handle comparison and show results
    if (clickCompareBtn) {
        if (score > 90) {
            PlaySound(Error);
            selectedCongratPhrase = congratPhrases[rand() % 5];
            showCongratMessage = true;
        }
        else {
            PlaySound(buttonClick);
            showCongratMessage = false;
        }
        flag = true;
    }
    
    // Draw results display area
    DrawRectangleRounded((Rectangle){ screenWidth / 2.0f - 500, 580, 1000, 400 }, 0.1f, 10, resultBoxBG);

    // Display appropriate message based on current state
    const char* resultText;
    if (!file1Selected || !file2Selected) {
        resultText = "Please select two files to compare";
    } else if (!flag) {
        resultText = "Press 'Compare Files' to analyze for plagiarism";
    } else {
        // Format and display similarity score
        char num_string[40];
        sprintf(num_string, "AND THE SIMILARITY SCORE IS %0.2f", score);
        int length = strlen(num_string);
        num_string[length]= '%';
        num_string[length+1] = '\0';
        resultText = num_string;
        
        // Draw similarity score
        Vector2 resultSize = MeasureTextEx(customFont, resultText, fontSize, 1.0f);
        DrawTextEx(customFont, resultText,
            (Vector2){ screenWidth / 2.0f - resultSize.x / 2, 700 }, fontSize, 1.0f, lightText);
        
        // Display congratulatory message for high similarity scores
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
