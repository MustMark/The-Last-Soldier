#include <iostream>
#include <fstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#define MaxEnemy 10
#define MaxItem 500

sf::Clock ButtonClock;
sf::Clock SelectClock;
sf::Clock NameClock;
sf::Clock ShootClock;

bool SoundMute = false, MusicMute = false;

sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game",sf::Style::Fullscreen);

sf::Music MenuSoundTrack;

// Main Game

int game(std::string PlayerName, int PlayerCharacter)
{
    // -Setting

    srand(time(NULL));

    // -Define Variables

    char PlayerDirection = 'd',BulletDirection;
    int Pscore = 0, ammo = 30, mag = 180, ptx = 0, pty = 80, EnemySpawnCount = 0, etx[MaxEnemy], ety[MaxEnemy], ic = 0, item[MaxEnemy];
    int EnemyType[MaxEnemy], OverShowCount = 0, PauseSelect = 0, iSkillCD = 0;
    bool BulletActive = false, ReloadActive = false, ItemActive[MaxItem], EnemyStop = false, DashActive = false, PushEnemy = false;
    bool BulletUpgrade = false, PauseActive = false;
    float PlayerSpeed = 0.12f, EnemySpeed[4], BulletSpeed = 5, php = 100, ani1 = 1, ani2 = 1, ani3 = 1, ani4 = 1, reload = 0;
    float eani = 1, ehp[MaxEnemy], SkillCD = 0, EnemyStopTime = 0, BulletUpgradeTime = 0;

    EnemySpeed[0] = 0.08;
    EnemySpeed[1] = 0.07;
    EnemySpeed[2] = 0.06;
    EnemySpeed[3] = 0.05;

    for (int i = 0; i < MaxEnemy; i++) {

        etx[i] = 0;
        ety[i] = 80;
    }

    for (int i = 0; i < MaxItem; i++) {

        ItemActive[i] = false;
    }

    // -Load

    sf::Texture PlayerSkin1;
    PlayerSkin1.loadFromFile("Resources/Textures/PlayerSkin1.png");

    sf::Texture PlayerSkin2;
    PlayerSkin2.loadFromFile("Resources/Textures/PlayerSkin2.png");

    sf::Texture PlayerSkin3;
    PlayerSkin3.loadFromFile("Resources/Textures/PlayerSkin3.png");

    sf::Texture PlayerSkin4;
    PlayerSkin4.loadFromFile("Resources/Textures/PlayerSkin4.png");

    sf::Texture PlayerSkin5;
    PlayerSkin5.loadFromFile("Resources/Textures/PlayerSkin5.png");

    sf::Texture Med;
    Med.loadFromFile("Resources/Textures/Med.png");

    sf::Texture tMag;
    tMag.loadFromFile("Resources/Textures/Mag.png");

    sf::Texture Zombie;
    Zombie.loadFromFile("Resources/Textures/Zombie.png");

    sf::Texture Skeleton;
    Skeleton.loadFromFile("Resources/Textures/Skeleton.png");

    sf::Texture ZombieUpgrade;
    ZombieUpgrade.loadFromFile("Resources/Textures/ZombieUpgrade.png");

    sf::Texture SkeletonUpgrade;
    SkeletonUpgrade.loadFromFile("Resources/Textures/SkeletonUpgrade.png");

    sf::Texture Map;
    Map.loadFromFile("Resources/Screens/Background.jpg");

    sf::Texture Black;
    Black.loadFromFile("Resources/Screens/Black.png");

    sf::Font RenegadePursuit;
    RenegadePursuit.loadFromFile("Resources/Fonts/RenegadePursuit-ywMr5.ttf");

    sf::Font Arial;
    Arial.loadFromFile("Resources/Fonts/arial.ttf");

    sf::Texture ShowPlayerSkin1;
    ShowPlayerSkin1.loadFromFile("Resources/Textures/ShowPlayerSkin1.jpg");

    sf::Texture ShowPlayerSkin2;
    ShowPlayerSkin2.loadFromFile("Resources/Textures/ShowPlayerSkin2.jpg");

    sf::Texture ShowPlayerSkin3;
    ShowPlayerSkin3.loadFromFile("Resources/Textures/ShowPlayerSkin3.jpg");

    sf::Texture ShowPlayerSkin4;
    ShowPlayerSkin4.loadFromFile("Resources/Textures/ShowPlayerSkin4.jpg");

    sf::Texture ShowPlayerSkin5;
    ShowPlayerSkin5.loadFromFile("Resources/Textures/ShowPlayerSkin5.jpg");

    sf::SoundBuffer S1;
    S1.loadFromFile("Resources/Sounds/Choose.mp3");

    sf::SoundBuffer S2;
    S2.loadFromFile("Resources/Sounds/Select.mp3");

    sf::SoundBuffer Shoot;
    Shoot.loadFromFile("Resources/Sounds/Shoot.mp3");

    sf::SoundBuffer ReloadSound;
    ReloadSound.loadFromFile("Resources/Sounds/ReloadSound.mp3");

    sf::SoundBuffer EnemyDead;
    EnemyDead.loadFromFile("Resources/Sounds/EnemyDead.mp3");

    sf::SoundBuffer ItemDrop;
    ItemDrop.loadFromFile("Resources/Sounds/ItemDrop.mp3");

    sf::SoundBuffer ItemPickup;
    ItemPickup.loadFromFile("Resources/Sounds/ItemPickup.mp3");

    sf::SoundBuffer GameOverSound;
    GameOverSound.loadFromFile("Resources/Sounds/GameOverSound.mp3");
    
    sf::SoundBuffer FreezeSound;
    FreezeSound.loadFromFile("Resources/Sounds/FreezeSound.mp3");

    sf::SoundBuffer PowerSound;
    PowerSound.loadFromFile("Resources/Sounds/PowerSound.mp3");

    sf::SoundBuffer DashSound;
    DashSound.loadFromFile("Resources/Sounds/DashSound.mp3");

    sf::SoundBuffer PushSound;
    PushSound.loadFromFile("Resources/Sounds/PushSound.mp3");

    sf::SoundBuffer HealSound;
    HealSound.loadFromFile("Resources/Sounds/HealSound.mp3");

    sf::SoundBuffer SkillReady;
    SkillReady.loadFromFile("Resources/Sounds/SkillReady.mp3");

    sf::Texture GameOver;
    GameOver.loadFromFile("Resources/Screens/GameOver.png");

    sf::Texture Pause;
    Pause.loadFromFile("Resources/Screens/Pause.png");

    sf::Music SoundTrack;
    SoundTrack.openFromFile("Resources/Sounds/GameSoundTrack.mp3");

    // -Define Objects

    sf::Sound Cho;
    Cho.setBuffer(S1);
    Cho.setVolume(50);

    sf::Sound Sel;
    Sel.setBuffer(S2);
    Sel.setVolume(100);

    sf::Sound SShoot;
    SShoot.setBuffer(Shoot);
    SShoot.setVolume(20);

    sf::Sound SReloadSound;
    SReloadSound.setBuffer(ReloadSound);
    SReloadSound.setVolume(50);

    sf::Sound SEnemyDead;
    SEnemyDead.setBuffer(EnemyDead);
    SEnemyDead.setVolume(30);

    sf::Sound SItemDrop;
    SItemDrop.setBuffer(ItemDrop);
    SItemDrop.setVolume(100);

    sf::Sound SItemPickup;
    SItemPickup.setBuffer(ItemPickup);
    SItemPickup.setVolume(50);

    sf::Sound SGameOverSound;
    SGameOverSound.setBuffer(GameOverSound);
    SGameOverSound.setVolume(50);

    sf::Sound SFreezeSound;
    SFreezeSound.setBuffer(FreezeSound);
    SFreezeSound.setVolume(30);

    sf::Sound SPowerSound;
    SPowerSound.setBuffer(PowerSound);
    SPowerSound.setVolume(30);

    sf::Sound SDashSound;
    SDashSound.setBuffer(DashSound);
    SDashSound.setVolume(30);

    sf::Sound SPushSound;
    SPushSound.setBuffer(PushSound);
    SPushSound.setVolume(50);

    sf::Sound SHealSound;
    SHealSound.setBuffer(HealSound);
    SHealSound.setVolume(30);

    sf::Sound SSkillReady;
    SSkillReady.setBuffer(SkillReady);
    SSkillReady.setVolume(100);

    SoundTrack.setVolume(25);

    if (SoundMute) {

        Cho.setVolume(0);
        Sel.setVolume(0);
        SShoot.setVolume(0);
        SReloadSound.setVolume(0);
        SEnemyDead.setVolume(0);
        SItemDrop.setVolume(0);
        SItemPickup.setVolume(0);
        SGameOverSound.setVolume(0);
        SFreezeSound.setVolume(0);
        SPowerSound.setVolume(0);
        SDashSound.setVolume(0);
        SPushSound.setVolume(0);
        SHealSound.setVolume(0);
        SSkillReady.setVolume(0);
    }
    if (MusicMute) {

        SoundTrack.setVolume(0);
    }

    sf::RectangleShape Player(sf::Vector2f(75.f,75.f));
    if (PlayerCharacter == 0) {

        Player.setTexture(&PlayerSkin1);
    }
    else if (PlayerCharacter == 1) {

        Player.setTexture(&PlayerSkin2);
    }
    else if (PlayerCharacter == 2) {

        Player.setTexture(&PlayerSkin3);
    }
    else if (PlayerCharacter == 3) {

        Player.setTexture(&PlayerSkin4);
    }
    else if (PlayerCharacter == 4) {

        Player.setTexture(&PlayerSkin5);
    }

    Player.setTextureRect(sf::IntRect(0, 0, 60, 80));
    Player.setOrigin(Player.getLocalBounds().width / 2, Player.getLocalBounds().height / 2);
    Player.setPosition(960, 540);

    sf::RectangleShape PlayerHitBlock(sf::Vector2f(45.f, 70.f));
    PlayerHitBlock.setFillColor(sf::Color::Red);
    PlayerHitBlock.setOrigin(PlayerHitBlock.getLocalBounds().width / 2, PlayerHitBlock.getLocalBounds().height / 2);
    PlayerHitBlock.setPosition(Player.getPosition().x, Player.getPosition().y);

    sf::RectangleShape Credits(sf::Vector2f(1920.f, 1080.f));
    Credits.setTexture(&Black);
    Credits.setPosition(0, 0);

    sf::RectangleShape ShowGameOver(sf::Vector2f(1920.f, 1080.f));
    ShowGameOver.setTexture(&GameOver);
    ShowGameOver.setPosition(0, 0);

    sf::RectangleShape ShowCharacter(sf::Vector2f(75.f, 75.f));
    ShowCharacter.setFillColor(sf::Color::White);
    ShowCharacter.setPosition(50, 12.5);

    if (PlayerCharacter == 0) {

        ShowCharacter.setTexture(&ShowPlayerSkin1);
    }
    else if (PlayerCharacter == 1) {

        ShowCharacter.setTexture(&ShowPlayerSkin2);
    }
    else if (PlayerCharacter == 2) {

        ShowCharacter.setTexture(&ShowPlayerSkin3);
    }
    else if (PlayerCharacter == 3) {

        ShowCharacter.setTexture(&ShowPlayerSkin4);
    }
    else if (PlayerCharacter == 4) {

        ShowCharacter.setTexture(&ShowPlayerSkin5);
    }

    sf::CircleShape Bullet(5.f);
    Bullet.setFillColor(sf::Color::Yellow);
    Bullet.setOrigin(Bullet.getLocalBounds().width / 2, Bullet.getLocalBounds().height / 2);

    sf::RectangleShape Enemy[MaxEnemy];
    for (int i = 0; i < MaxEnemy ; i++) {

        int ret = rand() % 100;

        if (ret >= 0 && ret <= 49) {

            Enemy[i].setTexture(&Zombie);
            EnemyType[i] = 0;
            ehp[i] = 100;

            Enemy[i].setSize(sf::Vector2f(65.f, 65.f));
            Enemy[i].setOrigin(Enemy[i].getLocalBounds().width / 2, Enemy[i].getLocalBounds().height / 2);
        }
        else if (ret >= 50 && ret <= 84) {

            Enemy[i].setTexture(&Skeleton);
            EnemyType[i] = 1;
            ehp[i] = 200;

            Enemy[i].setSize(sf::Vector2f(70.f, 70.f));
            Enemy[i].setOrigin(Enemy[i].getLocalBounds().width / 2, Enemy[i].getLocalBounds().height / 2);
        }
        else if (ret >= 85 && ret <= 94) {

            Enemy[i].setTexture(&ZombieUpgrade);
            EnemyType[i] = 2;
            ehp[i] = 500;

            Enemy[i].setSize(sf::Vector2f(90.f, 90.f));
            Enemy[i].setOrigin(Enemy[i].getLocalBounds().width / 2, Enemy[i].getLocalBounds().height / 2);
        }
        else if (ret >= 95 && ret <= 99 ) {

            Enemy[i].setTexture(&SkeletonUpgrade);
            EnemyType[i] = 3;
            ehp[i] = 1000;

            Enemy[i].setSize(sf::Vector2f(100.f, 100.f));
            Enemy[i].setOrigin(Enemy[i].getLocalBounds().width / 2, Enemy[i].getLocalBounds().height / 2);
        }

        Enemy[i].setTextureRect(sf::IntRect(0, 0, 60, 80));
    }

    sf::RectangleShape Item[MaxItem];
    for (int i = 0; i < MaxItem; i++) {

        Item[i].setSize(sf::Vector2f(30.f, 30.f));
        Item[i].setOrigin(Item[i].getLocalBounds().width / 2, Item[i].getLocalBounds().height / 2);
    }

    sf::RectangleShape EnemyHealth[MaxEnemy];

    sf::RectangleShape Background(sf::Vector2f(1920.f, 1080.f));
    Background.setTexture(&Map);

    sf::RectangleShape PauseBackground(sf::Vector2f(1920.f, 1080.f));
    PauseBackground.setTexture(&Pause);

    // -Main

    SoundTrack.play();
    SoundTrack.setLoop(true);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --Text

        sf::Text ScoreText;
        ScoreText.setFont(RenegadePursuit);
        ScoreText.setString("Score : ");
        ScoreText.setCharacterSize(24);
        ScoreText.setFillColor(sf::Color::White);
        ScoreText.setPosition(1500, 15);

        sf::Text Score;
        Score.setFont(RenegadePursuit);
        Score.setString(std::to_string(Pscore));
        Score.setCharacterSize(24);
        Score.setFillColor(sf::Color::White);
        Score.setPosition(1600, 15);

        sf::Text AmmoText;
        AmmoText.setFont(RenegadePursuit);
        AmmoText.setString("Ammo :");
        AmmoText.setCharacterSize(24);
        AmmoText.setFillColor(sf::Color::White);
        AmmoText.setPosition(1500, 60);

        sf::Text AmmoSlash;
        AmmoSlash.setFont(RenegadePursuit);
        AmmoSlash.setString("/");
        AmmoSlash.setCharacterSize(24);
        AmmoSlash.setFillColor(sf::Color::White);
        AmmoSlash.setPosition(1635, 60);

        sf::Text Ammo;
        Ammo.setFont(RenegadePursuit);
        Ammo.setString(std::to_string(ammo));
        Ammo.setCharacterSize(24);
        Ammo.setFillColor(sf::Color::White);
        Ammo.setPosition(1600, 60);

        sf::Text Mag;
        Mag.setFont(RenegadePursuit);
        Mag.setString(std::to_string(mag));
        Mag.setCharacterSize(24);
        Mag.setFillColor(sf::Color::White);
        Mag.setPosition(1650, 60);

        sf::Text Reload;
        Reload.setFont(RenegadePursuit);
        Reload.setString("Reloading...");
        Reload.setCharacterSize(24);
        Reload.setFillColor(sf::Color::White);
        Reload.setPosition(1600, 60);

        sf::Text CharacterText;
        CharacterText.setFont(RenegadePursuit);
        CharacterText.setString("Character : ");
        CharacterText.setCharacterSize(24);
        CharacterText.setFillColor(sf::Color::White);
        CharacterText.setPosition(30, 1000);

        sf::Text ShowCharacterText;
        ShowCharacterText.setFont(RenegadePursuit);
        if (PlayerCharacter == 0) {

            ShowCharacterText.setString("PRASERT");
        }
        else if (PlayerCharacter == 1) {

            ShowCharacterText.setString("ANAN");
        }
        else if (PlayerCharacter == 2) {

            ShowCharacterText.setString("BOONLERT");
        }
        else if (PlayerCharacter == 3) {

            ShowCharacterText.setString("SOMSAK");
        }
        else if (PlayerCharacter == 4) {

            ShowCharacterText.setString("JENJIRA");
        }
        ShowCharacterText.setCharacterSize(24);
        ShowCharacterText.setFillColor(sf::Color::Cyan);
        ShowCharacterText.setPosition(200, 1000);

        sf::Text Skill;
        Skill.setFont(RenegadePursuit);
        Skill.setString("Skill : ");
        Skill.setCharacterSize(24);
        Skill.setFillColor(sf::Color::White);
        Skill.setPosition(30, 1030);

        sf::Text SkillActive;
        SkillActive.setFont(RenegadePursuit);
        SkillActive.setString("Active");
        SkillActive.setCharacterSize(24);
        SkillActive.setFillColor(sf::Color::Green);
        SkillActive.setPosition(125, 1030);

        sf::Text SkillNotActive;
        SkillNotActive.setFont(RenegadePursuit);
        SkillNotActive.setString(std::to_string(iSkillCD));
        SkillNotActive.setCharacterSize(24);
        SkillNotActive.setFillColor(sf::Color::Red);
        SkillNotActive.setPosition(128, 1030);

        sf::Text Name;
        Name.setFont(RenegadePursuit);
        Name.setString("Name : ");
        Name.setCharacterSize(24);
        Name.setFillColor(sf::Color::Yellow);
        Name.setPosition(155, 10);

        sf::Text NamePlayer;
        NamePlayer.setFont(Arial);
        NamePlayer.setString(PlayerName);
        NamePlayer.setCharacterSize(24);
        NamePlayer.setFillColor(sf::Color::Yellow);
        NamePlayer.setPosition(245, 10);

        sf::Text PauseMenu[3];
        PauseMenu[0].setFont(RenegadePursuit);
        PauseMenu[0].setFillColor(sf::Color::Red);
        PauseMenu[0].setCharacterSize(30);
        PauseMenu[0].setString("BackToGame");
        PauseMenu[0].setOrigin(PauseMenu[0].getLocalBounds().width / 2, PauseMenu[0].getLocalBounds().height / 2);
        PauseMenu[0].setPosition(sf::Vector2f(960, 490));

        PauseMenu[1].setFont(RenegadePursuit);
        PauseMenu[1].setFillColor(sf::Color::White);
        PauseMenu[1].setCharacterSize(30);
        PauseMenu[1].setString("Restart");
        PauseMenu[1].setOrigin(PauseMenu[1].getLocalBounds().width / 2, PauseMenu[1].getLocalBounds().height / 2);
        PauseMenu[1].setPosition(sf::Vector2f(960, 540));

        PauseMenu[2].setFont(RenegadePursuit);
        PauseMenu[2].setFillColor(sf::Color::White);
        PauseMenu[2].setCharacterSize(30);
        PauseMenu[2].setString("Main Menu");
        PauseMenu[2].setOrigin(PauseMenu[2].getLocalBounds().width / 2, PauseMenu[2].getLocalBounds().height / 2);
        PauseMenu[2].setPosition(sf::Vector2f(960, 590));

        // --Input

            // ---Keyboard

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && Player.getPosition().y >= 140) {
                PlayerDirection = 'u';
                if (ani1 >= 1) {
                    Player.setTextureRect(sf::IntRect(ptx * 60, pty * 3, 60, 80));
                    ptx++;
                    if (ptx == 4) {

                        ptx = 0;
                    }
                    ani1 = 0;
                }
                ani1 += 0.005;
                ani2 = 1, ani3 = 1, ani4 = 1;
                Player.move(0, -PlayerSpeed);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && Player.getPosition().x >= 45) {
                PlayerDirection = 'l';
                if (ani2 >= 1) {
                    Player.setTextureRect(sf::IntRect(ptx * 60, pty * 1, 60, 80));
                    ptx++;
                    if (ptx == 4) {

                        ptx = 0;
                    }
                    ani2 = 0;
                }
                ani2 += 0.005;
                ani1 = 1, ani3 = 1, ani4 = 1;
                Player.move(-PlayerSpeed, 0);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && Player.getPosition().y <= 940) {
                PlayerDirection = 'd';
                if (ani3 >= 1) {
                    Player.setTextureRect(sf::IntRect(ptx * 60, pty * 0, 60, 80));
                    ptx++;
                    if (ptx == 4) {

                        ptx = 0;
                    }
                    ani3 = 0;
                }
                ani3 += 0.005;
                ani1 = 1, ani2 = 1, ani4 = 1;
                Player.move(0, PlayerSpeed);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && Player.getPosition().x <= 1865) {
                PlayerDirection = 'r';
                if (ani4 >= 1) {
                    Player.setTextureRect(sf::IntRect(ptx * 60, pty * 2, 60, 80));
                    ptx++;
                    if (ptx == 4) {

                        ptx = 0;
                    }
                    ani4 = 0;
                }
                ani4 += 0.005;
                ani1 = 1, ani2 = 1, ani3 = 1;
                Player.move(PlayerSpeed, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && mag > 0 && ammo < 30 || ammo == 0 && mag > 0) {
                
                SReloadSound.play();
                ReloadActive = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {

                if (SkillCD <= 0) {

                    if (PlayerCharacter == 0) {
                        
                        SFreezeSound.play();
                        EnemyStop = true;

                        for (int i = 0; i < MaxEnemy; i++) {

                            Enemy[i].setFillColor(sf::Color::Cyan);
                        }

                        PlayerSpeed = 0.2f;
                        EnemyStopTime = 0;

                        SkillCD = 15;
                    }
                    else if (PlayerCharacter == 1) {
                        
                        SPowerSound.play();
                        BulletUpgrade = true;
                        Player.setFillColor(sf::Color::Red);
                        Bullet.setFillColor(sf::Color::Red);
                        BulletUpgradeTime = 0;

                        SkillCD = 15;
                    }
                    else if (PlayerCharacter == 2) {

                        SDashSound.play();
                        DashActive = true;

                        SkillCD = 15;
                    }
                    else if (PlayerCharacter == 3) {

                        SPushSound.play();
                        PushEnemy = true;

                        SkillCD = 15;
                    }
                    else if (PlayerCharacter == 4 && php < 100) {

                        SHealSound.play();
                        if (php <= 75) {

                            php += 25;
                            SkillCD = 15;
                        }
                        else if (php > 75) {

                            php = 100;
                            SkillCD = 15;
                        }
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !BulletActive && ammo > 0 && !ReloadActive && ShootClock.getElapsedTime().asMilliseconds() >= 150) {
                    
                SShoot.play();
                BulletActive = true;
                BulletDirection = PlayerDirection;
                Bullet.setPosition(Player.getPosition().x, Player.getPosition().y);
                ammo--;

                ShootClock.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

                PauseActive = true;

                while (PauseActive) {

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && SelectClock.getElapsedTime().asMilliseconds() >= 300) {

                        Cho.play();
                        if (PauseSelect > 0) {

                            PauseMenu[PauseSelect].setFillColor(sf::Color::White);
                            PauseSelect--;
                            PauseMenu[PauseSelect].setFillColor(sf::Color::Red);
                        }
                        else if (PauseSelect == 0) {

                            PauseMenu[PauseSelect].setFillColor(sf::Color::White);
                            PauseSelect = 2;
                            PauseMenu[PauseSelect].setFillColor(sf::Color::Red);
                        }
                        
                        SelectClock.restart();
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && SelectClock.getElapsedTime().asMilliseconds() >= 300) {

                        Cho.play();
                        if (PauseSelect < 2) {

                            PauseMenu[PauseSelect].setFillColor(sf::Color::White);
                            PauseSelect++;
                            PauseMenu[PauseSelect].setFillColor(sf::Color::Red);
                        }
                        else if (PauseSelect == 2) {

                            PauseMenu[PauseSelect].setFillColor(sf::Color::White);
                            PauseSelect = 0;
                            PauseMenu[PauseSelect].setFillColor(sf::Color::Red);
                        }

                        SelectClock.restart();
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && ButtonClock.getElapsedTime().asMilliseconds() >= 300) {

                        Sel.play();
                        if (PauseSelect == 0) {

                            PauseActive = false;
                        }
                        else if (PauseSelect == 1) {

                            return 2;
                        }
                        else if (PauseSelect == 2) {

                            return 3;
                        }
                        
                        ButtonClock.restart();
                        ShootClock.restart();
                    }

                    window.draw(PauseBackground);

                    for (int i = 0; i < 3; i++) {

                        window.draw(PauseMenu[i]);
                    }

                    window.display();
                }
            }

        // --Objects
        
            // ---Bullet

                // ----Bullet.Move

                if (BulletActive) {

                    if (BulletDirection == 'u') {

                        Bullet.move(0, -BulletSpeed);
                        if (Bullet.getPosition().y < 100) {
                            BulletActive = false;
                        }
                    }
                    if (BulletDirection == 'l') {

                        Bullet.move(-BulletSpeed, 0);
                        if (Bullet.getPosition().x < 0) {
                            BulletActive = false;
                        }
                    }
                    if (BulletDirection == 'd') {

                        Bullet.move(0, BulletSpeed);
                        if (Bullet.getPosition().y > 980) {
                            BulletActive = false;
                        }
                    }
                    if (BulletDirection == 'r') {

                        Bullet.move(BulletSpeed, 0);
                        if (Bullet.getPosition().x > 1920) {
                            BulletActive = false;
                        }
                    }
                }

                // ----Bullet.Reload

                if (ReloadActive) {

                    if (reload > 1) {

                        if (mag < 30) {

                            if (ammo == 0) {
                                ammo = mag;
                                mag = 0;
                            }
                            if (ammo > 0) {

                                if (ammo + mag > 30) {

                                    mag = mag - 30 + ammo;
                                    ammo = 30;

                                }
                                if (ammo + mag <= 30) {

                                    ammo = ammo + mag;
                                    mag = 0;
                                }
                            }
                        }
                        if (mag >= 30) {

                            if (ammo == 0) {
                                ammo = 30;
                                mag -= 30;
                            }
                            if (ammo > 0) {
                                mag = mag - 30 + ammo;
                                ammo = 30;
                            }
                        }

                        ReloadActive = false;
                        reload = 0;
                    }
                    reload += 0.00023;
                }

            // ---Enemy

                // ----Enemy.Spawn

                if (EnemySpawnCount == 0) {

                    for (int i = 0; i < MaxEnemy; i++) {
                    
                        int g1;

                        g1 = rand() % 4;

                        if (g1 == 0) {

                            Enemy[i].setPosition(rand() % 1821 + 50, 50);
                        }

                        else if (g1 == 1) {

                            Enemy[i].setPosition(rand() % 1821 + 50, 1030);
                        }
                        else if (g1 == 2) {

                            Enemy[i].setPosition(-50, rand() % 781 + 150);
                        }
                        else if (g1 == 3) {

                            Enemy[i].setPosition(1970, rand() % 781 + 150);
                        }
                    }

                    EnemySpawnCount++;
                }

                // ----Enemy.Move

                if (!EnemyStop) {

                    for (int i = 0; i < MaxEnemy; i++) {

                        float tpx = Player.getPosition().x;
                        float tpy = Player.getPosition().y;

                        float tex = Enemy[i].getPosition().x;
                        float tey = Enemy[i].getPosition().y;

                        float dis = sqrt((tpx - tex) * (tpx - tex) + (tpy - tey) * (tpy - tey));
                        float nex = (tpx - tex) / dis;
                        float ney = (tpy - tey) / dis;

                        if (EnemyType[i] == 0) {

                            Enemy[i].move(nex* EnemySpeed[0], ney * EnemySpeed[0]);
                        }
                        else if (EnemyType[i] == 1) {

                            Enemy[i].move(nex * EnemySpeed[1], ney * EnemySpeed[1]);
                        }
                        else if (EnemyType[i] == 2) {

                            Enemy[i].move(nex * EnemySpeed[2], ney * EnemySpeed[2]);
                        }
                        else if (EnemyType[i] == 3) {

                            Enemy[i].move(nex * EnemySpeed[3], ney * EnemySpeed[3]);
                        }

                        if (tex < tpx) {

                            if (eani >= 1) {

                                Enemy[i].setTextureRect(sf::IntRect(etx[i] * 60, ety[i] * 2, 60, 80));
                                etx[i]++;
                                if (etx[i] == 4) {

                                    etx[i] = 0;
                                }
                                eani = 0;
                            }
                            eani += 0.005;
                        }
                        if (tex > tpx) {

                            if (eani >= 1) {

                                Enemy[i].setTextureRect(sf::IntRect(etx[i] * 60, ety[i] * 1, 60, 80));
                                etx[i]++;
                                if (etx[i] == 4) {

                                    etx[i] = 0;
                                }
                                eani = 0;
                            }
                            eani += 0.005;
                        }

                        EnemyHealth[i].setPosition(tex, tey - 30);
                    }
                }
                else if (EnemyStop) {

                    if (EnemyStopTime >= 1) {

                        EnemyStop = false;

                        for (int i = 0; i < MaxEnemy; i++) {

                            Enemy[i].setFillColor(sf::Color::White);
                        }

                        PlayerSpeed = 0.12f;
                        EnemyStopTime = 0;
                    }
                    else {

                        EnemyStopTime += 0.0005;
                    }
                }
                
                if (PushEnemy) {

                    for (int i = 0; i < MaxEnemy; i++) {

                        float tpx = Player.getPosition().x;
                        float tpy = Player.getPosition().y;

                        float tex = Enemy[i].getPosition().x;
                        float tey = Enemy[i].getPosition().y;

                        if (abs(tpx - tex) < 400 && abs(tpy - tey) < 400) {

                            if (tpx > tex) {

                                Enemy[i].move(-400,0);
                            }
                            else {

                                Enemy[i].move(400, 0);
                            }
                        }
                    }
                    
                    PushEnemy = false;
                }

                // ----Enemy.Heath

                for (int i = 0; i < MaxEnemy; i++) {

                    if (EnemyType[i] == 0) {

                        EnemyHealth[i].setSize(sf::Vector2f(ehp[i] / 3, 8.f));
                    }
                    else if (EnemyType[i] == 1) {

                        EnemyHealth[i].setSize(sf::Vector2f(ehp[i] / 6, 8.f));
                    }
                    else if (EnemyType[i] == 2) {

                        EnemyHealth[i].setSize(sf::Vector2f(ehp[i] / 15, 8.f));
                    }
                    else if (EnemyType[i] == 3) {

                        EnemyHealth[i].setSize(sf::Vector2f(ehp[i] / 30, 8.f));
                    }

                    EnemyHealth[i].setOrigin(EnemyHealth[i].getLocalBounds().width / 2, EnemyHealth[i].getLocalBounds().height / 2);
                    EnemyHealth[i].setFillColor(sf::Color::Red);
                }

            // ---Player
                
                if (DashActive) {

                    if (PlayerDirection == 'u') {
                        if (Player.getPosition().y - 300 < 140) {

                            Player.setPosition(Player.getPosition().x, 140);
                        }
                        else {

                            Player.move(0, -300);
                        }
                    }
                    if (PlayerDirection == 'l') {
                        if (Player.getPosition().x - 300 < 45) {

                            Player.setPosition(45, Player.getPosition().y);
                        }
                        else {

                            Player.move(-300, 0);
                        }
                    }
                    if (PlayerDirection == 'd') {
                        if (Player.getPosition().y + 300 > 940) {

                            Player.setPosition(Player.getPosition().x, 940);
                        }
                        else {

                            Player.move(0, 300);
                        }
                    }
                    if (PlayerDirection == 'r') {
                        if (Player.getPosition().x + 300 > 1865) {

                            Player.setPosition(1865, Player.getPosition().y);
                        }
                        else {

                            Player.move(300, 0);
                        }
                    }

                    DashActive = false;
                }

            // ---Collision

                // ----Collision.BulletEnemy

                for (int i = 0; i < MaxEnemy ; i++) {

                    if (Bullet.getGlobalBounds().intersects(Enemy[i].getGlobalBounds())) {

                        if (BulletActive) {

                            if (BulletUpgrade) {

                                ehp[i] -= 75;
                            }
                            else if (!BulletUpgrade) {

                                ehp[i] -= 50;
                            }
                            BulletActive = false;
                        }
                    }
                }

                // ----Collision.PlayerEnemy

                for (int i = 0; i < MaxEnemy; i++) {

                    if (PlayerHitBlock.getGlobalBounds().intersects(Enemy[i].getGlobalBounds())) {

                        php -= 0.02;
                    }
                }

                // ----Collision.PlayerItem

                for (int i = 0; i < MaxItem; i++) {

                    if (PlayerHitBlock.getGlobalBounds().intersects(Item[i].getGlobalBounds())) {

                        SItemPickup.play();
                        if (item[i] == 0) {

                            if (php <= 75) {

                                php += 25;
                            }
                            else if (php > 75) {

                                php = 100;
                            }
                        }
                        else if (item[i] == 1) {

                            mag += 30;
                        }
                        ItemActive[i] = false;
                        Item[i].setPosition(0, 0);
                    }
                }

            //-- CheckEnemy

            for (int i = 0; i < MaxEnemy; i++) {
                
                if (ehp[i] <= 0) {

                    SEnemyDead.play();
                    if (EnemyType[i] == 0) {

                        Pscore += rand() % 11 + 10;
                    }
                    else if (EnemyType[i] == 1) {

                        Pscore += rand() % 21 + 20;
                    }
                    else if (EnemyType[i] == 2) {

                        Pscore += rand() % 26 + 75;
                    }
                    else if (EnemyType[i] == 3) {

                        Pscore += rand() % 51 + 150;
                    }
                        
                    int rditem = rand() % 10;

                    if (rditem == 0) {

                        SItemDrop.play();
                        item[ic] = 0;
                        Item[ic].setPosition(Enemy[i].getPosition().x, Enemy[i].getPosition().y);
                        Item[ic].setTexture(&Med);
                        ItemActive[ic] = true;
                        ic++;
                    }
                    else if (rditem == 1) {

                        SItemDrop.play();
                        item[ic] = 1;
                        Item[ic].setPosition(Enemy[i].getPosition().x, Enemy[i].getPosition().y);
                        Item[ic].setTexture(&tMag);
                        ItemActive[ic] = true;
                        ic++;
                    }

                    int g1;;

                    g1 = rand() % 4;

                    if (g1 == 0) {

                        Enemy[i].setPosition(rand() % 1821 + 50, 50);
                    }
                    else if (g1 == 1) {

                        Enemy[i].setPosition(rand() % 1821 + 50, 1030);
                    }
                    else if (g1 == 2) {

                        Enemy[i].setPosition(-50, rand() % 781 + 150);
                    }
                    else if (g1 == 3) {

                        Enemy[i].setPosition(1970, rand() % 781 + 150);
                    }

                    int ret = rand() % 100;

                    if (ret >= 0 && ret <= 49) {

                        Enemy[i].setTexture(&Zombie);
                        EnemyType[i] = 0;
                        ehp[i] = 100;

                        Enemy[i].setSize(sf::Vector2f(60.f, 60.f));
                        Enemy[i].setOrigin(Enemy[i].getLocalBounds().width / 2, Enemy[i].getLocalBounds().height / 2);
                    }
                    else if (ret >= 50 && ret <= 84) {

                        Enemy[i].setTexture(&Skeleton);
                        EnemyType[i] = 1;
                        ehp[i] = 200;

                        Enemy[i].setSize(sf::Vector2f(70.f, 70.f));
                        Enemy[i].setOrigin(Enemy[i].getLocalBounds().width / 2, Enemy[i].getLocalBounds().height / 2);
                    }
                    else if (ret >= 85 && ret <= 94) {

                        Enemy[i].setTexture(&ZombieUpgrade);
                        EnemyType[i] = 2;
                        ehp[i] = 500;

                        Enemy[i].setSize(sf::Vector2f(90.f, 90.f));
                        Enemy[i].setOrigin(Enemy[i].getLocalBounds().width / 2, Enemy[i].getLocalBounds().height / 2);
                    }
                    else if (ret >= 95 && ret <= 99) {

                        Enemy[i].setTexture(&SkeletonUpgrade);
                        EnemyType[i] = 3;
                        ehp[i] = 1000;

                        Enemy[i].setSize(sf::Vector2f(100.f, 100.f));
                        Enemy[i].setOrigin(Enemy[i].getLocalBounds().width / 2, Enemy[i].getLocalBounds().height / 2);
                    }

                    EnemyHealth[i].setPosition(Enemy[i].getPosition().x, Enemy[i].getPosition().y - 30);
                }
            }

        // --ObjectUpdate

        sf::RectangleShape PlayerHealth(sf::Vector2f(php * 10, 25.f));
        PlayerHealth.setFillColor(sf::Color::Green);
        PlayerHealth.setPosition(165, 50);

        PlayerHitBlock.setPosition(Player.getPosition().x, Player.getPosition().y);

        if (BulletUpgrade) {

            if (BulletUpgradeTime >= 1) {

                Player.setFillColor(sf::Color::White);
                Bullet.setFillColor(sf::Color::Yellow);
                BulletUpgrade = false;
                BulletUpgradeTime = 0;
            }
            else {

                BulletUpgradeTime += 0.00025;
            }
        }


        SkillCD -= 0.0005;
        iSkillCD = SkillCD;

        if (iSkillCD == 0) {

            SSkillReady.play();
        }

        // --Display

        window.clear();
        
            // ---Screen

            window.draw(Background);

            // ---Object

            window.draw(Player);

            if (BulletActive) {

                window.draw(Bullet);
            }

            for (int i = 0; i < MaxEnemy; i++) {

                window.draw(Enemy[i]);
                window.draw(EnemyHealth[i]);
            }

            for (int i = 0; i < MaxItem; i++) {

                if (ItemActive[i]) {

                    window.draw(Item[i]);
                }
            }

            // ---Info

            window.draw(Credits);
            window.draw(ScoreText);
            window.draw(Score);
            window.draw(AmmoText);
            window.draw(ShowCharacter);
            window.draw(CharacterText);
            window.draw(ShowCharacterText);
            window.draw(Name);
            window.draw(NamePlayer);
            window.draw(Skill);
            window.draw(PlayerHealth);

            if (SkillCD <= 0) {

                window.draw(SkillActive);
            }
            else if (SkillCD > 0) {

                window.draw(SkillNotActive);
            }

            if (ReloadActive) {

                window.draw(Reload);
            }
            else if (!ReloadActive) {

                window.draw(AmmoSlash);
                window.draw(Ammo);
                window.draw(Mag);
            }

            while (php < 0) {
                
                if (OverShowCount == 0) {

                    SoundTrack.stop();
                    SGameOverSound.play();
                    ShowCharacter.setPosition(960, 500);
                    ShowCharacter.setSize(sf::Vector2f(80.f, 80.f));
                    ShowCharacter.setOrigin(ShowCharacter.getLocalBounds().width / 2, ShowCharacter.getLocalBounds().height / 2);

                    ShowCharacterText.setCharacterSize(20);
                    ShowCharacterText.setFillColor(sf::Color::White);
                    ShowCharacterText.setStyle(sf::Text::Bold);
                    ShowCharacterText.setPosition(960, 550);
                    ShowCharacterText.setOrigin(ShowCharacterText.getLocalBounds().width / 2, ShowCharacterText.getLocalBounds().height / 2);

                    NamePlayer.setCharacterSize(60);
                    NamePlayer.setFillColor(sf::Color::Black);
                    NamePlayer.setPosition(860, 595);

                    Score.setFont(Arial);
                    Score.setCharacterSize(60);
                    Score.setFillColor(sf::Color::Black);
                    Score.setPosition(860, 665);

                    window.draw(ShowGameOver);
                    window.draw(ShowCharacterText);
                    window.draw(NamePlayer);
                    window.draw(ShowCharacter);
                    window.draw(Score);
                    window.display();

                    OverShowCount++;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

                    std::ofstream file("Resources/Score.txt", std::ios::app);

                    file << PlayerName << " " << PlayerCharacter << " " << Pscore << "\n";
                    file.close();

                    return 1;
                }
            }

        window.display();
    }

    return 0;
}

int playerinfo() {

    // -Define Variables

    int Stage = 0, character = 2;
    std::string str;

    // -Load

    sf::SoundBuffer S1;
    S1.loadFromFile("Resources/Sounds/Choose.mp3");

    sf::SoundBuffer S2;
    S2.loadFromFile("Resources/Sounds/Select.mp3");

    sf::Texture Bg;
    Bg.loadFromFile("Resources/Screens/PlayerInfo_Background.jpg");

    sf::Texture Info;
    Info.loadFromFile("Resources/Screens/CharacterInfo.png");

    sf::Font Arial;
    Arial.loadFromFile("Resources/Fonts/arial.ttf");

    sf::Texture ShowPlayerSkin1;
    ShowPlayerSkin1.loadFromFile("Resources/Textures/ShowPlayerSkin1.jpg");

    sf::Texture ShowPlayerSkin2;
    ShowPlayerSkin2.loadFromFile("Resources/Textures/ShowPlayerSkin2.jpg");

    sf::Texture ShowPlayerSkin3;
    ShowPlayerSkin3.loadFromFile("Resources/Textures/ShowPlayerSkin3.jpg");
    
    sf::Texture ShowPlayerSkin4;
    ShowPlayerSkin4.loadFromFile("Resources/Textures/ShowPlayerSkin4.jpg");
    
    sf::Texture ShowPlayerSkin5;
    ShowPlayerSkin5.loadFromFile("Resources/Textures/ShowPlayerSkin5.jpg");

    // -Define Objects

    sf::Sound Cho;
    Cho.setBuffer(S1);
    Cho.setVolume(50);

    sf::Sound Sel;
    Sel.setBuffer(S2);
    Sel.setVolume(100);

    if (SoundMute) {

        Cho.setVolume(0);
        Sel.setVolume(0);
    }

    sf::RectangleShape Background(sf::Vector2f(1920.f, 1080.f));
    Background.setTexture(&Bg);

    sf::RectangleShape CInfo(sf::Vector2f(1920.f, 1080.f));
    CInfo.setTexture(&Info);

    sf::RectangleShape SCH[5];
    for (int i = 0; i < 5; i++) {

        SCH[i].setSize(sf::Vector2f(200.f, 200.f));
        SCH[i].setPosition(1920 / 6 * (i + 1), 600);
        SCH[i].setOrigin(SCH[i].getLocalBounds().width / 2, SCH[i].getLocalBounds().height / 2);
    }


    SCH[0].setTexture(&ShowPlayerSkin1);
    SCH[1].setTexture(&ShowPlayerSkin2);
    SCH[2].setTexture(&ShowPlayerSkin3);
    SCH[3].setTexture(&ShowPlayerSkin4);
    SCH[4].setTexture(&ShowPlayerSkin5);

    sf::RectangleShape SelectCharacter;
    SelectCharacter.setSize(sf::Vector2f(240.f, 400.f));
    SelectCharacter.setPosition((1920 / 6) * 3, 680);
    SelectCharacter.setFillColor(sf::Color::Red);
    SelectCharacter.setOrigin(SelectCharacter.getLocalBounds().width / 2, SelectCharacter.getLocalBounds().height / 2);

    // -Main

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // --Text

        sf::Text Name;
        Name.setFont(Arial);
        Name.setStyle(sf::Text::Bold);
        Name.setCharacterSize(40);
        Name.setFillColor(sf::Color::Black);
        Name.setPosition(960, 230);

        // --Input

            // ---Keyboard
            
            if (Stage == 0 ){

                if (sf::Event::TextEntered == event.type && NameClock.getElapsedTime().asMilliseconds() >= 150) {

                    if (event.text.unicode == 8 && str.size() > 0) {

                        str.erase(str.size() - 1, 1);
                    }
                    else if (event.text.unicode >= 33 && event.text.unicode <= 122 && str.size() < 12) {

                        str += static_cast<char>(event.text.unicode);
                    }

                    NameClock.restart();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && str.size() > 0 && ButtonClock.getElapsedTime().asMilliseconds() >= 300) {

                    Sel.play();
                    Stage++;

                    ButtonClock.restart();
                }
            }
            else if (Stage == 1) {

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && SelectClock.getElapsedTime().asMilliseconds() >= 300) {

                    Cho.play();
                    if (character != 4) {

                        SelectCharacter.move(1920 / 6, 0);
                        character++;
                    }
                    else if (character == 4) {

                        SelectCharacter.move(- (1920 / 6 * 4), 0);
                        character = 0;
                    }

                    SelectClock.restart();
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && SelectClock.getElapsedTime().asMilliseconds() >= 300) {

                    Cho.play();
                    if (character != 0) {

                        SelectCharacter.move(-(1920 / 6), 0);
                        character--;
                    }
                    else if (character == 0) {

                        SelectCharacter.move(1920 / 6 * 4, 0);
                        character = 4;
                    }

                    SelectClock.restart();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && ButtonClock.getElapsedTime().asMilliseconds() >= 300) {

                    Sel.play();
                    ShootClock.restart();
                    int Play = game(str, character);

                    if (Play == 2) {

                        return 2;
                    }
                    else if (Play == 1 || Play == 3) {

                        return 1;
                    }

                    ButtonClock.restart();
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

                return 1;
            }

        // --ObjectUpdate

        Name.setString(str);
        Name.setOrigin(Name.getLocalBounds().width / 2, Name.getLocalBounds().height / 2);

        // --Display

        window.clear();

            // ---Screen

            window.draw(Background);

            // ---Info

            window.draw(Name);

            if (Stage == 1) {

                window.draw(SelectCharacter);
            }

            for (int i = 0; i < 5; i++) {

                window.draw(SCH[i]);
            }

            window.draw(CInfo);

        window.display();
    }
    return 0;
}

// Main HighScore

int highscore() {

    // -Define Variables

    int Select = 0;

    struct HighScore {

        std::string Name;
        int Character;
        int Score;
    };

    std::vector<HighScore> highScores;

    // -Load

    sf::Texture Bg;
    Bg.loadFromFile("Resources/Screens/Menu50.png");

    sf::Texture Bg2;
    Bg2.loadFromFile("Resources/Screens/HighScore.png");

    sf::Font RenegadePursuit;
    RenegadePursuit.loadFromFile("Resources/Fonts/RenegadePursuit-ywMr5.ttf");

    sf::Font Arial;
    Arial.loadFromFile("Resources/Fonts/arial.ttf");

    std::ifstream file("Resources/Score.txt");

    HighScore score;

    while (file >> score.Name >> score.Character >> score.Score) {

        highScores.push_back(score);
    }

    file.close();

    // -Define Objects

    sf::RectangleShape Background1(sf::Vector2f(1920.f, 1080.f));
    Background1.setFillColor(sf::Color::White);

    sf::RectangleShape Background2(sf::Vector2f(1920.f, 1080.f));
    Background2.setTexture(&Bg);

    sf::RectangleShape Background3(sf::Vector2f(1920.f, 1080.f));
    Background3.setTexture(&Bg2);

    for (size_t i = 0; i < highScores.size(); i++) {

        for (size_t j = i + 1; j < highScores.size(); j++) {

            if (highScores[i].Score < highScores[j].Score) {

                std::swap(highScores[i], highScores[j]);
            }
        }
    }

    // -Main

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --Input

            // ---Keyboard

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

            return 0;
        }

        // --Display

        window.clear();

            // ---Screen

            window.draw(Background1);
            window.draw(Background2);
            window.draw(Background3);

            sf::Text ShowName;
            ShowName.setFont(Arial);
            ShowName.setCharacterSize(60);
            ShowName.setStyle(sf::Text::Bold);
            ShowName.setFillColor(sf::Color::White);

            sf::Text ShowCharacter;
            ShowCharacter.setFont(RenegadePursuit);
            ShowCharacter.setCharacterSize(60);
            ShowCharacter.setStyle(sf::Text::Bold);
            ShowCharacter.setFillColor(sf::Color::White);

            sf::Text ShowScore;
            ShowScore.setFont(RenegadePursuit);
            ShowScore.setCharacterSize(60);
            ShowScore.setStyle(sf::Text::Bold);
            ShowScore.setFillColor(sf::Color::White);

            int limit = 0;
            float px1 = 755, px2 = 1150, px3 = 1490,py = 260;

            if (highScores.size() < 10) {

                limit = highScores.size();
            }
            else if (highScores.size() >= 10) {

                limit = 10;
            }

            for (size_t i = 0; i < limit; i++) {

                if (i >= 3) {

                    ShowName.setCharacterSize(30);
                    ShowCharacter.setCharacterSize(30);
                    ShowScore.setCharacterSize(30);
                }

                ShowName.setString(highScores[i].Name);
                ShowName.setPosition(px1, py);

                if (highScores[i].Character == 0){

                    ShowCharacter.setString("PRASERT");
                }
                else if (highScores[i].Character == 1) {

                    ShowCharacter.setString("ANAN");
                }
                else if (highScores[i].Character == 2) {

                    ShowCharacter.setString("BOONLERT");
                }
                else if (highScores[i].Character == 3) {

                    ShowCharacter.setString("SOMSAK");
                }
                else if (highScores[i].Character == 4) {

                    ShowCharacter.setString("JENJIRA");
                }

                ShowCharacter.setPosition(px2, py);

                ShowScore.setString(std::to_string(highScores[i].Score));
                ShowScore.setPosition(px3, py);

                ShowName.setOrigin(ShowName.getLocalBounds().width / 2, ShowName.getLocalBounds().height / 2);
                ShowCharacter.setOrigin(ShowCharacter.getLocalBounds().width / 2, ShowCharacter.getLocalBounds().height / 2);
                ShowScore.setOrigin(ShowScore.getLocalBounds().width / 2, ShowScore.getLocalBounds().height / 2);

                window.draw(ShowName);
                window.draw(ShowCharacter);
                window.draw(ShowScore);

                if (i < 2) {

                    py += 100.0f;
                }
                else if (i == 2) {

                    py += 90.0f;
                }
                else if (i > 2) {

                    py += 65.0f;
                }
            }

            window.display();
    }

    return 0;
}

// Main HowToPlay

int howtoplay() {

    // -Load

    sf::Texture Bg;
    Bg.loadFromFile("Resources/Screens/Menu50.png");

    sf::Texture Bg2;
    Bg2.loadFromFile("Resources/Screens/HowToPlay.png");

    sf::Font font;
    font.loadFromFile("Resources/Fonts/BruceForeverRegular-X3jd2.ttf");

    // -Define Objects

    sf::RectangleShape Background1(sf::Vector2f(1920.f, 1080.f));
    Background1.setFillColor(sf::Color::White);

    sf::RectangleShape Background2(sf::Vector2f(1920.f, 1080.f));
    Background2.setTexture(&Bg);

    sf::RectangleShape Background3(sf::Vector2f(1920.f, 1080.f));
    Background3.setTexture(&Bg2);

    // -Main

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --Input

            // ---Keyboard

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

            return 0;
        }

        // --Display

        window.clear();

            // ---Screen

            window.draw(Background1);
            window.draw(Background2);
            window.draw(Background3);

            window.display();
    }

    return 0;
}

// Main Setting

int setting() {

    // -Define Variables

    int Select = 0;

    // -Load

    sf::Texture Bg;
    Bg.loadFromFile("Resources/Screens/Menu50.png");

    sf::Texture Bg2;
    Bg2.loadFromFile("Resources/Screens/setting.png");

    sf::SoundBuffer S1;
    S1.loadFromFile("Resources/Sounds/Choose.mp3");

    sf::SoundBuffer S2;
    S2.loadFromFile("Resources/Sounds/Select.mp3");

    sf::Font RenegadePursuit;
    RenegadePursuit.loadFromFile("Resources/Fonts/RenegadePursuit-ywMr5.ttf");

    // -Define Objects

    sf::Sound Cho;
    Cho.setBuffer(S1);
    Cho.setVolume(50);

    sf::Sound Sel;
    Sel.setBuffer(S2);
    Sel.setVolume(100);

    sf::RectangleShape Background1(sf::Vector2f(1920.f, 1080.f));
    Background1.setFillColor(sf::Color::White);

    sf::RectangleShape Background2(sf::Vector2f(1920.f, 1080.f));
    Background2.setTexture(&Bg);

    sf::RectangleShape Background3(sf::Vector2f(1920.f, 1080.f));
    Background3.setTexture(&Bg2);

    // -Text

    sf::Text menu[2];
    menu[0].setFont(RenegadePursuit);
    menu[0].setFillColor(sf::Color::Red);
    menu[0].setCharacterSize(50);
    if (MusicMute) {

        menu[0].setString("MUSIC : CLOSE");
    }
    else if (!MusicMute) {

        menu[0].setString("MUSIC : OPEN");
    }
    menu[0].setPosition(sf::Vector2f(960, 400));
    menu[0].setOrigin(menu[0].getLocalBounds().width / 2, menu[0].getLocalBounds().height / 2);

    menu[1].setFont(RenegadePursuit);
    menu[1].setFillColor(sf::Color::White);
    menu[1].setCharacterSize(50);
    if (SoundMute) {

        menu[1].setString("SOUND : CLOSE");
    }
    else if (!SoundMute) {

        menu[1].setString("SOUND : OPEN");
    }
    menu[1].setPosition(sf::Vector2f(960, 500));
    menu[1].setOrigin(menu[1].getLocalBounds().width / 2, menu[1].getLocalBounds().height / 2);

    // -Main

    while (window.isOpen())
    {
        Cho.setVolume(50);
        Sel.setVolume(100);
        MenuSoundTrack.setVolume(100);

        if (SoundMute) {

            Cho.setVolume(0);
            Sel.setVolume(0);
        }
        if (MusicMute) {

            MenuSoundTrack.setVolume(0);
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --Input

            // ---Keyboard

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && SelectClock.getElapsedTime().asMilliseconds() >= 300) {

                Cho.play();
                if (Select > 0) {

                    menu[Select].setFillColor(sf::Color::White);
                    Select--;
                    menu[Select].setFillColor(sf::Color::Red);
                }
                else if (Select == 0) {

                    menu[Select].setFillColor(sf::Color::White);
                    Select = 1;
                    menu[Select].setFillColor(sf::Color::Red);
                }

                SelectClock.restart();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && SelectClock.getElapsedTime().asMilliseconds() >= 300) {

                Cho.play();
                if (Select < 1) {

                    menu[Select].setFillColor(sf::Color::White);
                    Select++;
                    menu[Select].setFillColor(sf::Color::Red);
                }
                else if (Select == 1) {

                    menu[Select].setFillColor(sf::Color::White);
                    Select = 0;
                    menu[Select].setFillColor(sf::Color::Red);
                }

                SelectClock.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && SelectClock.getElapsedTime().asMilliseconds() >= 300) {

                Sel.play();
                if (Select == 0) {

                    if (MusicMute) {

                        MusicMute = false;
                        menu[0].setString("MUSIC : OPEN");
                        menu[0].setOrigin(menu[0].getLocalBounds().width / 2, menu[0].getLocalBounds().height / 2);
                    }
                    else if (!MusicMute) {

                        MusicMute = true;
                        menu[0].setString("MUSIC : CLOSE");
                        menu[0].setOrigin(menu[0].getLocalBounds().width / 2, menu[0].getLocalBounds().height / 2);
                    }
                }
                else if (Select == 1) {

                    if (SoundMute) {

                        SoundMute = false;
                        menu[1].setString("SOUND : OPEN");
                        menu[1].setOrigin(menu[1].getLocalBounds().width / 2, menu[1].getLocalBounds().height / 2);
                    }
                    else if (!SoundMute) {

                        SoundMute = true;
                        menu[1].setString("SOUND : CLOSE");
                        menu[1].setOrigin(menu[1].getLocalBounds().width / 2, menu[1].getLocalBounds().height / 2);
                    }
                }

                SelectClock.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {


                return 0;
            }

        // --Display

        window.clear();

            // ---Screen

            window.draw(Background1);
            window.draw(Background2);
            window.draw(Background3);

            for (int i = 0; i < 2; i++) {

                window.draw(menu[i]);
            }

            window.display();
    }

    return 0;
}

// Main Menu

int WinMain() {

    // -Define Variables

    int Select = 0;
    bool Restart = false;

    // -Load

    sf::Texture Bg;
    Bg.loadFromFile("Resources/Screens/Menu.jpg");

    sf::Font font;
    font.loadFromFile("Resources/Fonts/BruceForeverRegular-X3jd2.ttf");

    sf::SoundBuffer S1;
    S1.loadFromFile("Resources/Sounds/Choose.mp3");

    sf::SoundBuffer S2;
    S2.loadFromFile("Resources/Sounds/Select.mp3");

    MenuSoundTrack.openFromFile("Resources/Sounds/MenuSoundTrack.mp3");

    // -Define Objects

    sf::RectangleShape Background(sf::Vector2f(1920.f, 1080.f));
    Background.setTexture(&Bg);

    sf::Sound Cho;
    Cho.setBuffer(S1);
    Cho.setVolume(50);

    sf::Sound Sel;
    Sel.setBuffer(S2);
    Sel.setVolume(100);

    // -Text

    sf::Text menu[5];
    menu[0].setFont(font);
    menu[0].setFillColor(sf::Color::Red);
    menu[0].setCharacterSize(100);
    menu[0].setString("Play");
    menu[0].setPosition(sf::Vector2f(50, 300));

    menu[1].setFont(font);
    menu[1].setFillColor(sf::Color::White);
    menu[1].setString("HighScore");
    menu[1].setPosition(sf::Vector2f(50, 450));

    menu[2].setFont(font);
    menu[2].setFillColor(sf::Color::White);
    menu[2].setString("HowToPlay");
    menu[2].setPosition(sf::Vector2f(50, 540));

    menu[3].setFont(font);
    menu[3].setFillColor(sf::Color::White);
    menu[3].setString("Setting");
    menu[3].setPosition(sf::Vector2f(50, 630));

    menu[4].setFont(font);
    menu[4].setFillColor(sf::Color::White);
    menu[4].setString("Quit");
    menu[4].setPosition(sf::Vector2f(50, 720));

    // -Main

    MenuSoundTrack.play();
    MenuSoundTrack.setLoop(true);

    while (window.isOpen())
    {
        Cho.setVolume(50);
        Sel.setVolume(100);

        if (SoundMute) {

            Cho.setVolume(0);
            Sel.setVolume(0);
        }
        if (MusicMute) {

            MenuSoundTrack.setVolume(0);
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --Input

            // ---Keyboard

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && SelectClock.getElapsedTime().asMilliseconds() >= 300) {

                Cho.play();
                if (Select > 0) {

                    menu[Select].setFillColor(sf::Color::White);
                    Select--;
                    menu[Select].setFillColor(sf::Color::Red);
                }
                else if (Select == 0) {

                    menu[Select].setFillColor(sf::Color::White);
                    Select=4;
                    menu[Select].setFillColor(sf::Color::Red);
                }

                SelectClock.restart();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && SelectClock.getElapsedTime().asMilliseconds() >= 300) {

                Cho.play();
                if (Select < 4) {

                    menu[Select].setFillColor(sf::Color::White);
                    Select++;
                    menu[Select].setFillColor(sf::Color::Red);
                }
                else if (Select == 4) {

                    menu[Select].setFillColor(sf::Color::White);
                    Select = 0;
                    menu[Select].setFillColor(sf::Color::Red);
                }

                SelectClock.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && ButtonClock.getElapsedTime().asMilliseconds() >= 300) {

                Sel.play();
                if (Select == 0 ) {

                    MenuSoundTrack.stop();

                    Restart = true;

                    while (Restart) {

                        int EndNum = playerinfo();

                        if (EndNum == 1) {

                            Restart = false;
                            MenuSoundTrack.play();
                        }
                        else if (EndNum == 2) {

                            Restart = true;
                        }
                    }
                }
                else if (Select == 1) {

                    highscore();
                }
                else if (Select == 2) {

                    howtoplay();
                }
                else if (Select == 3) {

                    SelectClock.restart();
                    setting();
                }
                else if (Select == 4) {

                    window.close();
                }

                ButtonClock.restart();
            }

        // --Display

        window.clear();
            
            // ---Screen

            window.draw(Background);

            // ---Info

            for (int i = 0; i < 5; i++) {

            window.draw(menu[i]);
            }

        window.display();
    }

    return 0;
}