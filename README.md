# BOF : L'objet Interdit

Un petit jeu en **C/C++ avec raylib** pour découvrir les **buffer overflows** de façon ludique et pédagogique.

## 🎮 Concept
Vous incarnez un personnage pouvant parler à des PNJ, gérer un petit inventaire et tenter d’obtenir **L'Objet Interdit**.  
Le problème ? **L'Objet Interdit** est indispensable pour terminer le jeu, mais il semble impossible à récupérer par des moyens « normaux ».  
Saurez‑vous quand même finir le jeu ?

## 🚀 Compilation & Exécution
```bash
# Cloner le dépôt
git clone https://github.com/IAidenI/bof-objet-interdit
cd bof-objet-interdit

# Compiler
make clean && make

# Lancer le jeu
./bofgame
```

***⚠️ATTENTION : la taille de l'écran doit être au minimum de 1280x960***

## 🎯 Objectif du joueur
- Explorer le jeu  
- Découvrir le bug  
- Terminer le jeu

## 📌 État du projet (TODO / DONE)
### ✅ Fait
- Prototype (PoC) en console fonctionnel : lecture du nom, remplissage d'inventaire, appel des fonctions `inventory().add()`, `inventory().hasEnoughOf()` et affichage d'informations. (bofgame_command_line)
- Définitions d'entités de base (`Player`, `Item`, `Inventory`, `Hitbox`, `Position`) et headers dans `headers/`.
- Menu permettant de visualiser le contenu de la stack (en ligne de commande)
- Main minimal démontrant la vulnérabilité.
- Système de dialogues avec choix (version simple pour l'instant) implémenté.
- Premier PoC avec UI réalisé, pour se faire, récupèrer 32 patates, les renommer en CCCCCCCCCCCCCCCC puis voir le garde
- Asset pour le background
- Visuel de la stack

### 🔜 À faire / Améliorations prévues
**Urgent :**
- Faire le dialogue avec le fermier
- Affichage de la stack : ajout pour le survol de la souris, des informations complémentaire (A quoi correspond la valeur, sa correspondance en ASCII etc...)
- Application trop lent à charger à cause des font/textures qui chargent

**A faire :**
- Refactor logique et design des dialogues
- Documentation pédagogique dans `/docs` expliquant la faille et comment elle fonctionne.
- Peut être un menu start/end

**Optionelles :**
- Ajouter un bandeau invormatif (par exemple en bas à droite) pour indiquer des informations comme x1 patate récupèré, inventaire plein...
- Peut être améliorer des assets existantes
- Survol de la souris dans l'inventaire affiche les infos de l'item

## ⚠️ Avertissement
Ce projet contient volontairement une vulnérabilité à des fins **pédagogiques**. **Ne reproduisez pas** ces techniques sur des systèmes réels ou en production sans autorisation explicite. Utilisez ce projet uniquement pour apprendre et expérimenter dans un environnement contrôlé.
