-- Entity
enemy = {
    -- Entity components
    transform = {
         X = 1200,
         Y = 1400,
         scaleX = 3.0,
         scaleY = 3.0

    },
    physics = {
    	mass = 1000.0,
        velocityX = 0.0,
        velocityY = 0.0,
        aceclerationX = 0.0,
        accelerationY = 0.0,
        gravityX = 0.0,
        gravityY = 0.0,
        friction = 3.0
    },
    sprite = {
        filename = "assets/slime.png",
        id = "slime",
        animation = {
            default = {
                name = "default",
                row = 0,
                numFrames = 4
            },
        },
        width = 32,
        height = 32
    },
    collider = {
    	width = 32,
    	height = 35,
        type = "npc"
    },
    HP = 20
-- you can also have comments
}
