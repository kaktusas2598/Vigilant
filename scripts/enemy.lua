-- Entity
enemy = {
    -- Entity components
    transform = {
         X = 100,
         Y = 100,
         scaleX = 3.0,
         scaleY = 1.0

    },
    physics = {
        velocityX = 0.0,
        velocityY = 0.0,
        aceclerationX = 0.0,
        accelerationY = 0.0,
        gravityX = 0.0,
        gravityY = 0.0
    },
    sprite = {
        filename = "assets/player.png",
        id = "player",
        animation = {
            default = {
                name = "default",
                row = 0,
                numFrames = 6
            },
            run = {
                name = "run",
                row = 1,
                numFrames = 6
            },
            fight = {
                name = "fight",
                row = 2,
                numFrames = 4
            },
            fall = {
                name = "fall",
                row = 4,
                numFrames = 3
            }
        },
        width = 48,
        height = 48
    },
    collider = {
        type = "npc"
    },
    HP = 20
-- you can also have comments
}