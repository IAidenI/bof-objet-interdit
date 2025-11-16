#include "manager.hpp"

bool CheckFileExiste(const char *path) {
    if (path == nullptr) {
        cerr << "[ ERREUR ] Chemin null détecté." << endl;
        return false;
    }

    if (!FileExists(path)) {
        cerr << "[ ERREUR ] Fichier introuvable : " << path << endl;
        return false;
    }

    return true;
}

bool Manager::init() {
    // ---- Check si les fichiers sont présents ----
    for (const char* texPath : textures) {
        if (!CheckFileExiste(texPath)) {
            this->valid = false;
            return this->valid;
        }
    }

    for (const char* fontPath : fonts) {
        if (!CheckFileExiste(fontPath)) {
            this->valid = false;
            return this->valid;
        }
    }

    // ---- Initialisation des textures ----
    for (int i = 0; i < TEX_MAX; i++) {
        cout << "Texture : " << this->textures[i] << endl;
        this->tmgr[i] = LoadTexture(this->textures[i]);
    }

    // ---- Initialisation de fonts ----
    vector<int> cps;
    cps.reserve(size(FR_CHARS));
    for (char32_t ch : FR_CHARS) cps.push_back((int)ch);
    
    for (int i = 0; i < FONT_MAX; ++i) {
        fmgr[i][0] = LoadFontEx(this->fonts[i], SMALL_SIZE, cps.data(), cps.size());
        SetTextureFilter(fmgr[i][0].texture, TEXTURE_FILTER_BILINEAR);

        fmgr[i][1] = LoadFontEx(this->fonts[i], BIG_SIZE, cps.data(), cps.size());
        SetTextureFilter(fmgr[i][1].texture, TEXTURE_FILTER_BILINEAR);
    }
    return this->valid;
}

const Font& Manager::getFont(FontID id, int size) {
    int index = (size < SMALL_SIZE) ? 0 : 1;
    return this->fmgr[id][index];
}

Manager::~Manager() {
    if (!this->valid) return;

    // Déchargement des textures
    for (int i = 0; i < TEX_MAX; i++) {
        UnloadTexture(this->tmgr[i]);
    }

    // Déchargement des fonts
    for (int i = 0; i < FONT_MAX; i++) {
        for (int j = 0; j < FONT_SIZE_COUNT; j++) {
            UnloadFont(this->fmgr[i][j]);
        }
    }
}
