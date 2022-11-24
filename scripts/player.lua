-- Entity
player = {
    -- Entity components
    transform = {
         X = 100,
         Y = 400,
    },
    sprite = {
        filename = "assets/player.png",
        animation = {},
        width = 48,
        height = 48,
    },
    HP = 20,
-- you can also have comments
}

-- TODO: call C++ functions/callbacks
-- Write Lua Wrappers for animation/render, physics, collision
-- 