#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <vector>
#include <cstdint>
#include "colors.hpp"
#include "item.hpp"
#include "manager.hpp"
#include "geometry.hpp"
#include "style.hpp"
#include "game_settings.hpp"

// ---- Constantes ----
// Dialogue
inline constexpr Size DEFAULT_ICON_SIZE = { 94.0f, 94.0f };

// Items
inline constexpr float DEFAULT_ITEM_SCALE  = INVENTORY_SCALE * 0.88f;
inline constexpr float DEFAULT_ITEM_RADIUS = 13.0f;

// ---- Position dans une frame ----
enum WindowPosition {
    TOP_LEFT,
    TOP_RIGHT,
    BOT_RIGHT,
    BOT_LEFT,
    CENTER
};

// ---- Informations pour dessiner une card ----
struct CardConfig {
    // Position dans la frame
    WindowPosition position = TOP_LEFT;

    // Général
    Color backgroundColor  = COLOR_STACK_GLOBAL_BACKGROUND;
    Color borderColor  = COLOR_STACK_BORDER;
    float stroke = DEFAULT_STROKE;

    // Titre
    float titleOffset = DEFAULT_TITLE_OFFSET;
    Padding titlePad  = DEFAULT_TITLE_PAD_IN;

    // Contenu
    Padding padIn   = DEFAULT_PAD_IN;
    Margins padOut  = DEFAULT_PAD_OUT;
    float interline = DEFAULT_INTERLINE;
};

struct Card {
    TextStyle title;
    vector<vector<InfoSegment>> content;
    CardConfig config;
};

struct CardLayout {
    Frame title;
    Frame content;
};

// Pour le hover dans la card
struct HoverValues {
    size_t offset = SIZE_MAX;
    Frame frame = { 0.0f };
};

// ---- Informations pour la section de texte ----
struct DataConfig {
    // Position dans la frame
    WindowPosition position = TOP_LEFT;

    // Général
    Color color    = COLOR_STACK_CARD_BACKGROUND;
    float stroke   = DEFAULT_STROKE;


    // Contenu
    Padding padIn   = DEFAULT_PAD_IN;
    Margins padOut  = DEFAULT_PAD_OUT;
    float interline = DEFAULT_INTERLINE;
};

struct DataSection {
    vector<vector<InfoSegment>> data;
    DataConfig config;
};

// ---- Structures animations ----
struct AnimationState {
    float frame = 0;
    float timer = 0.0f;
    bool facingRight = false;
    float frameTimeIdle = 0.12f;
    float frameTimeMove = 0.12f;
    float scale = 4.0f;
};

struct SpriteSheetInfo {
    float frameW;
    float frameH;
    float frameCols;
    float rowIdle;
    float rowMove;
};

// ---- Image de profile dialogue ----
struct IconProfile {
    Texture2D entity;
    AnimationState entityAnim;
    SpriteSheetInfo entitySprite;
    Color color;
};

// ---- Pour arrondir les frames et ne pas avoir des demi pixels ----
inline Frame AlignToPixels(Frame f) {
    f.x = roundf(f.x);
    f.y = roundf(f.y);
    f.width  = roundf(f.width);
    f.height = roundf(f.height);
    return f;
}

Size GetMaxLineSize(const vector<InfoSegment>& line); // Calcul les dimenssions maximal pour une ligne
Size GetFrameMaxSize(const vector<vector<InfoSegment>>& content, Padding padIn, float interline); // Calcul la taille maximale dont aura besoin une frame
Size GetFrameMaxSizeWithBranch(const vector<vector<InfoSegment>>& content, Padding padIn, float interline, float indentAfterTitle, float titleInterline); // Calcul la taille maximale dont aura besoin une frame en prenant en compte une branche

Frame GetFrame(const vector<vector<InfoSegment>>& content, WindowPosition position, Frame parentFrame, Padding padIn, Margins padOut, float interline); // Calcul la position et la taille d'une frame
Frame GetFrameTitle(const TextStyle& title, Frame parentFrame, WindowPosition parentPosition, float titleOffset, Padding padTitle, Margins padFrame, float stroke); // Calcul la position et la taille d'une frame pour un titre
CardLayout GetCardFrame(const Card& card, Frame parentFrame); // Calcul la frame d'une card (titre + contenu)
Frame GetInfoFrame(const vector<Card>& cards, const vector<DataSection>& dataSection, WindowPosition position, Frame parentFrame, Padding padIn, Margins padOut); // Calcul la frame pour la section information

HoverValues DrawCard(const Card& card, Frame parentFrame, float roundness = DEFAULT_ROUNDNESS, int segments = DEFAULT_SEGMENTS); // Permet de dessiner une card
void DrawDataSection(const DataSection& data, Frame parentFrame, float roundness = DEFAULT_SEGMENTS, int segments = DEFAULT_SEGMENTS); // Dessine une section avec des cards et des zones de textes
void DrawInfoSection(const vector<Card>& cards, const vector<DataSection>& dataSection, WindowPosition position, Frame parentFrame, Padding padIn = DEFAULT_PAD_IN, Margins padOut = DEFAULT_PAD_OUT, float interline = DEFAULT_INTERLINE, float stroke = DEFAULT_STROKE); // Dessine une section avec des cards et des zones de textes

void DrawInputBox(TextStyle name, int maxInputChars, Padding padIn = DEFAULT_PAD_IN, float stroke = DEFAULT_STROKE); // Dessine une saisie utilisateur
void DrawToolTip(const vector<vector<InfoSegment>>& data, Frame parentFrame, Padding padIn = DEFAULT_PAD_IN, float roundness = DEFAULT_ROUNDNESS, int segments = DEFAULT_SEGMENTS, float stroke = DEFAULT_STROKE, float interline = DEFAULT_INTERLINE); // Dessine un info-bulle pour un hover
void DrawItemToolTip(const vector<vector<InfoSegment>>& data, Frame parentFrame, Padding padIn = DEFAULT_PAD_IN, float roundness = DEFAULT_ROUNDNESS, int segments = DEFAULT_SEGMENTS, float stroke = DEFAULT_STROKE, float interline = DEFAULT_INTERLINE); // Dessine un info-bulle pour un item de l'inventaire

void DrawStaticItem(const Texture2D& texture, Position pos, float scale); // Dessine une image statique
void DrawAnimatedEntity(const Texture2D& texture, AnimationState& anim, Position pos, bool moving, const SpriteSheetInfo& entitySprite, Color color); // Dessine une image animé
void DrawInfoLabel(Hitbox entity, int entitySize, const TextStyle& text); // Dessine au dessus d'une entité une étiquette d'information
void DrawDialogue(const vector<vector<InfoSegment>>& data, const TextStyle& continueData, IconProfile& profile, Padding padInFrameContent = DEFAULT_TITLE_PAD_IN, Padding padInContent = DEFAULT_TITLE_PAD_IN, Size iconFrameSize = DEFAULT_ICON_SIZE, float roundness = DEFAULT_ROUNDNESS, int segments = DEFAULT_SEGMENTS, float stroke = DEFAULT_STROKE, float interline = DEFAULT_INTERLINE); // Dessine un bandeau pour un dialogue

void DrawCornerMarkers(const Rectangle& r, float len, float thick, Color color); // Dessine des marqueurs dans les angles d'un rectangle

Size GetButtonSize(TextStyle& data, Padding padIn); // Récupère la taille d'un bouton
bool DrawRoundedButton(TextStyle& data, Position position, bool active, Padding padIn = DEFAULT_PAD_IN, float roundness = DEFAULT_ROUNDNESS, int segments = DEFAULT_SEGMENTS, float stroke = DEFAULT_STROKE); // Dessine un bouton arroundi
void DrawRightArrow(Frame parentFrame, float thick, Color color); // Dessine une flèche qui pointe vers la droite
void DrawItemWithQuantity(Position pos, TextStyle& itemQuantity, const Texture2D& texture, float itemScale = DEFAULT_ITEM_SCALE, float radiusCircleInfo = DEFAULT_ITEM_RADIUS); // Dessine un item avec sa quantité en bas à droite
int DrawStore(int potatoCurrent, int carrotCurrent, int appleCurrent, Manager& manager, float roundness = DEFAULT_ROUNDNESS, int segments = DEFAULT_SEGMENTS, float stroke = DEFAULT_STROKE); // Dessine un magasin où l'utilisateur peut acheter des items
bool DrawTrade(Position start, int currentValue, int maxValue, int itemGetValue, Texture2D textureIn, Texture2D textureOut, float itemScale, Manager& manager); // Dessine une linge de trade avec item in, une flèche avec le ratio actuelle/max, l'item out et un bouton pour intéragir
void DrawNotificationBar(string data, Padding padIn = DEFAULT_PAD_IN, Margins padOut = DEFAULT_PAD_OUT); // Dessine une barre de notification en bas à gauche de l'écran (TODO passer en paramètre la position)

#endif // UTILS_H
