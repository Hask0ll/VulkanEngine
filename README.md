# VulkanEngine

## À propos
Ce projet est conçu pour accompagner les débutants dans l'apprentissage de l'API Vulkan en suivant les ressources du site [Vulkan-guide.com](https://vulkan-guide.com). Il propose une progression structurée, des ressources ciblées et des objectifs clairs pour maîtriser les concepts fondamentaux de cette API graphique moderne.

## Prérequis
- Connaissances en C/C++
- Notions de base en infographie 3D
- Carte graphique compatible Vulkan
- Compilateur C++ (GCC, Clang, MSVC)
- IDE (Visual Studio, VS Code, CLion)

## Installation
1. Installez le SDK Vulkan depuis [LunarG](https://www.lunarg.com/vulkan-sdk/)
2. Configurez les variables d'environnement:
   ```
   VULKAN_SDK=chemin/vers/votre/installation
   PATH=%PATH%;%VULKAN_SDK%\Bin
   ```
3. Vérifiez l'installation:
   ```
   vulkaninfo
   ```

## Structure du Projet
```
vulkan-learning-project/
├── src/                  # Code source
├── shaders/              # Fichiers shader GLSL
├── include/              # Fichiers d'en-tête
├── lib/                  # Bibliothèques
├── build/                # Fichiers de compilation
├── assets/               # Ressources (textures, modèles)
└── docs/                 # Documentation supplémentaire
```

## Guide de Progression

### Étape 1: Fondamentaux et Configuration
- Étudier l'architecture Vulkan
- Configurer l'environnement de développement
- Comprendre le cycle de vie d'une application Vulkan

### Étape 2: Initialisation de Vulkan
- Créer une instance Vulkan
- Sélectionner un périphérique physique
- Créer un périphérique logique et des files d'attente
- Établir une surface de présentation

### Étape 3: Rendu de Base
- Mettre en place une chaîne de permutation (swap chain)
- Créer un pipeline graphique
- Implémenter des shaders simples
- Dessiner un triangle

### Étape 4: Concepts Avancés
- Ajouter des textures et des tampons uniformes
- Implémenter des transformations
- Charger et afficher des modèles 3D
- Optimiser les performances

## Commandes de Compilation
```bash
# Créer le répertoire de build
mkdir -p build && cd build

# Générer les fichiers de compilation avec CMake
cmake ..

# Compiler le projet
make

# Exécuter l'application
./vulkan_app
```

## Compiler les Shaders
```bash
# Pour compiler un shader vertex
glslc shader.vert -o vert.spv

# Pour compiler un shader fragment
glslc shader.frag -o frag.spv
```

## Ressources Recommandées
- [Vulkan-guide.com](https://vulkan-guide.com) - Guide principal du projet
- [Vulkan Tutorial](https://vulkan-tutorial.com) - Ressource complémentaire détaillée
- [Khronos Vulkan Registry](https://www.khronos.org/registry/vulkan/) - Documentation officielle
- [Awesome Vulkan](https://github.com/vinjn/awesome-vulkan) - Collection de ressources Vulkan

## Dépannage Courant
- **Erreur de validation des couches** : Vérifiez que les couches de validation sont correctement installées et activées.
- **Problèmes de compatibilité matérielle** : Assurez-vous que votre carte graphique prend en charge Vulkan et que les pilotes sont à jour.
- **Erreurs de compilation de shader** : Vérifiez la syntaxe GLSL et la version utilisée.

## Contribution
Les suggestions d'amélioration et les contributions sont les bienvenues. Veuillez créer une issue ou une pull request pour toute proposition.

## Licence
Ce projet d'apprentissage est distribué sous licence MIT. Consultez le fichier LICENSE pour plus d'informations.

---

Bonne exploration de Vulkan ! N'hésitez pas à consulter régulièrement les ressources mises à jour sur [Vulkan-guide.com](https://vulkan-guide.com).
