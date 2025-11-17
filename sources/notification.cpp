#include "notification.hpp"

// Convertit une ligne en plusieurs en fonction d'une taille maximal
vector<string> Multilines(TextStyle data, float maxWidth) {
    vector<string> lines;
    string currentLine = "";
    string currentWord = "";

    auto measure = [&](const string& s) {
        return MeasureTextStyled({ data.font, s.c_str(), (float)data.font->baseSize, data.spacing, data.color }).x;
    };

    for (char c : data.text) {
        if (c == ' ') {
            if (measure(currentLine + currentWord + ' ') > maxWidth) {
                lines.push_back(currentLine);
                currentLine.clear();
            }
            currentLine += currentWord + ' ';
            currentWord.clear();
        } else {
            currentWord.push_back(c);
        }
    }

    if (!currentWord.empty()) {
        if (measure(currentLine + currentWord) > maxWidth) {
            lines.push_back(currentLine);
            currentLine = currentWord;
        } else {
            currentLine += currentWord;
        }
    }

    if (!currentLine.empty()) lines.push_back(currentLine);
    
    return lines;
}

// Configure les paramètres
void Notification::config(Frame parentFrame, float maxWidth, const Font& font, Padding padIn, Margins padOut) {
    this->parentFrame = parentFrame;
    this->maxWidth    = maxWidth;
    this->font        = &font;
    this->padIn       = padIn;
    this->padOut      = padOut;
    this->notifications.reserve(NOTIFICATION_LIMIT);
}

// Vérifie que la configuration à bien été faite
bool Notification::isValid() {
    if (this->font == nullptr) return false;
    if (this->font->baseSize <= 0) return false;
    if (this->maxWidth <= 0 || this->maxWidth <= 0) return false;
    if (this->parentFrame.width <= 0 || this->parentFrame.height <= 0) return false;

    return true;
}

// Dessine une barre de notification en bas à gauche de l'écran
void Notification::draw() {
    if (this->notifCount > NOTIFICATION_LIMIT) return;
    if (!this->isValid()) {
        cout << "[ WARNING ] Notification : Impossible d'afficher la notification, la méthode .config() n'as jamais été appelé." << endl;
        return;
    }

    float currentY = this->parentFrame.height - this->padOut.y;
    // On parcours les notifications en sens inverse pour mettre la plus récente en bas
    for (auto it = this->notifications.rbegin(); it != this->notifications.rend(); ++it) {
        const Notif &notif = *it;

        // Mesures
        TextStyle user = { this->font, notif.user + " > ", (float)this->font->baseSize, 2.0f, COLOR_NOTIFICATION_USER };
        Size userSize = MeasureTextStyled(user);
        
        TextStyle fullData = { this->font, notif.content, (float)this->font->baseSize, 2.0f, COLOR_NOTIFICATION_MESSAGE };
        vector<string> lines = Multilines(fullData, maxWidth - 2.0f * this->padIn.x - userSize.x);
        
        float lineHeight    = MeasureTextStyled(fullData).y;
        float contentHeight = lineHeight * lines.size();
        float frameHeight   = contentHeight + 2.0f * this->padIn.y;
        
        // Crée la frame
        Frame frame = {
            this->padOut.x,
            currentY - frameHeight,
            this->maxWidth,
            frameHeight,
        };

        currentY -= frameHeight + this->padOut.y;

        // Dessin du fond + bord
        long long nowMs = duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        long long elapsed = nowMs - notif.start;
        long long durationMs = NOTIFICATION_DURATION_MAX_SECONDS * 1000;

        // Le fade commence à 80% du temps
        float fadeStart = 0.8f; 

        float progress = (float)elapsed / (float)durationMs;
        float ratio = 1;

        if (progress > fadeStart) {
            float fadeT = (progress - fadeStart) / (1.0f - fadeStart);
            ratio = 1.0f - fadeT;
        }

        Color bg = COLOR_NOTIFICATION_BACKGROUND;
        bg.a                   = (unsigned char)(COLOR_NOTIFICATION_BACKGROUND.a * ratio);
        user.color.a           = (unsigned char)(COLOR_NOTIFICATION_USER.a * ratio);
        unsigned char alphaMsg = (unsigned char)(COLOR_NOTIFICATION_MESSAGE.a * ratio);
        DrawRectangleRec(frame, bg);

        // Dessine le contenu
        Position contentPosition = {
            frame.x + this->padIn.x,
            frame.y + this->padIn.y
        };

        DrawTextStyled(user, contentPosition);

        for (const auto& line : lines) {
            fullData.color.a = alphaMsg;
            TextStyle lineStyle = { this->font, line, (float)fullData.font->baseSize, fullData.spacing, fullData.color };
            DrawTextStyled(lineStyle, { contentPosition.x + userSize.x, contentPosition.y });
            contentPosition.y += lineHeight;
        }
    }
}

// Génère un id unique pour chaque notifications
int Notification::generateID() {
    for (int i = 0; i < NOTIFICATION_LIMIT; ++i) {
        bool used = false;
        for (const auto& notif : this->notifications) {
            if (notif.id == i) {
                used = true;
                break;
            }
        }

        if (!used) {
            // Si i n'est utilisé par aucune notif alors il est libre id libre
            return i;
        }
    }

    cout << "[ WARNING ] Notification : Le maximum de notification a été atteint." << std::endl;
    return -1;  // Plus d'id disponible
}

// Supprime de la list de notification si elle a expiré
void Notification::update() {
    long long nowMs = duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    auto it = remove_if(
        this->notifications.begin(),
        this->notifications.end(),
        [nowMs](const Notif& n) {
            return n.isExpired(nowMs);
        }
    );

    this->notifications.erase(it, this->notifications.end());
}

// Ajoute une notification
void Notification::push(string user, string message) {
    int id = this->generateID();
    if (id != -1) {
        long long nowMs = duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count(); // Date actuelle en millisecondes
        Notif notification = { id, user, message, nowMs };
        this->notifications.push_back(notification);
    }
}
