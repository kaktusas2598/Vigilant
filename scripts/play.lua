-- Available functions from Engine

-- Entity functions:
-- Entity.create(), use create() wrapper which adds entity to entities global table which Lua will use
-- Entity.move(x, y)
-- Entity.scale(x, y)
-- Entity.addSprite(id, filename, w, h)
-- Entity.addAnimation(name, row, numFrames)
-- Entity.setAnimation(name, once = false)
-- Entity.setAbsolutePosition(isAbsolute): takes boolean argument
-- Entity.addPhysics(mass, frictionCoefficient) : friction from 0.00 to 1.00
-- Entity.addCollider(width, height)
-- Entity.setColliderListener(funcName)
-- Entity.addInput()
-- Entity.setInputListener(funcName)
-- Entity.addButton()
-- Entity.setButtonListener(funcName)
-- Entity.addLabel(x, y, text, fontId, r, g, b, a)
-- Entity.setLabel(text)
-- Entity.setLabelAlignment(enum horizontal, enum vertical)
-- Entity.id()

-- General Functions:
-- playSound(id)
-- playMusic(id)
-- addParticleEmitter(originX, originY, type)
-- getMapWidth()
-- getMapHeight()
-- getScreenWidth()
-- getScreenHeight()
-- changeState(stateID)

-- Globals
-- Tried putting this in init script and loading it for all states, but it doesn't seem to work correctly
entities = {} -- All Lua Managed entities will be here sorted by id

function create()
    entity = Entity.create()
    entities[entity:id()] = entity
    return entity
end

-- Redefining lua print() so we can redirect output to debug console and logs if needed
luaPrint = print
print = function(...)
    --logStub = '[LUA]: '
    luaPrint(...)
    addLog(...)
    -- For some reason arg is always nil
    --for i,v in ipairs(arg) do
        --logStub = logStub..tostring(v)
    --end
    --addLog(logStub)
end

function define(data, tag)
    entity = create()
    -- Components must be setup and initialised in this order:
    -- Tranform
    -- Sprite
    -- Everything else in any order??
    if data["transform"] ~= nil then
        entity:move(data["transform"]['X'], data["transform"]['Y'])
        entity:scale(data["transform"]['scaleX'], data["transform"]['scaleY'])
    end
    if data["sprite"] ~= nil then
        -- Assume required properties are set for now
        entity:addSprite(data['sprite']['id'], data['sprite']['filename'], data['sprite']['width'], data['sprite']['height'])
        for k2,v2 in pairs(data['sprite']['animation']) do
            entity:addAnimation(v2['name'], v2['row'], v2['numFrames'])
        end
    end

    -- Careful when using pairs(), it returns elements in unspecified order and that's why
    -- I had to move out transform and sprite stuff above because it needs to be initialised first
    -- before components like collider
    for k, v in pairs(data) do
        if k == "id" then
            if (data[k]["type"]) then
                entity:setType(data[k]["type"])
            end
        end
        if k == "input" then
            entity:addInput()
            entity:setInputListener("onInput")
        end
        if k == "physics" then
            entity:addPhysics(v['mass'], v['friction'])
        end
        if k == "collider" then
            entity:addCollider(v['width'], v['height'])
            entity:setCollideListener("onCollide")
        end
    end
    entities[entity:id()] = entity
    return entity
end


-- Player event listeners
function onCollide(thisId, secondId)
    if entities[thisId]:getType() == "playerProjectile" then
        playSound("boom")
        entities[thisId]:remove()
        -- MASSIVE FPS DROPS HERE if calling this without removing second entity to stop collision dispatches
        addParticleEmitter(entities[secondId]:getX(), entities[secondId]:getY(), "burst")
    else -- player collided with someone
        if entities[secondId]:getType() == "enemy" then
        end
    end
end

function onInput(thisId, key)
    -- Update player position label
    playerPosLabel:setLabel("Player  Pos  "..math.floor(player:getX())..", "..math.floor(player:getY()))
    -- W
    if key == 119 then
        -- Does nit work for some reason, sprite stays in the same place
        -- Probably need to bind input manager for this
        --entities[thisId]:applyForceY(-10)
    end
    -- A
    if key == 97 then
        --entities[thisId]:applyForceX(-10)
    end
    -- S
    if key == 115 then
        --entities[thisId]:applyForceY(10)
    end
    -- D
    if key == 100 then
        --entities[thisId]:applyForceX(10)
    end
    -- F
    if key == 102 then
        entities[thisId]:setAnimation("hitRight", true)
        testEntity = create()
        testEntity:setType("playerProjectile")
        testEntity:move(entities[thisId]:getX(), entities[thisId]:getY())
        testEntity:addSprite("bullet", "assets/projectiles/bullet1.png", 10, 10)
        testEntity:addProjectile(entities[thisId], 150, 1000)
        playSound("phaser")
    end
    -- Space
    if key == 32 then
        playSound("jump")
        entities[thisId]:setAnimation("dash", true) -- ? why doesnt work?
    end
end

-- back to Main Menu callback and definition
function onMainMenuClick(thisId)
    playSound("button")
    changeState("MENU")
end
mainMenuButton = create()
mainMenuButton:move((getScreenWidth() - 200), 0)
mainMenuButton:addSprite("mainmenubutton", "", 200, 80)
mainMenuButton:setAbsolutePosition(true)
mainMenuButton:addButton()
mainMenuButton:setButtonListener("onMainMenuClick")

-- Projectile template
projectileTable = {
}


-- Entity
playerTable = {
    -- Entity components
    id = {
        type = "player"
    },
    transform = {
        X = 100,
        Y = 250,
        scaleX = 2.0,
        scaleY = 2.0

    },
    physics = {
        mass = 1.0,
        velocityX = 0.0,
        velocityY = 0.0,
        aceclerationX = 0.0,
        accelerationY = 0.0,
        gravityX = 0.0,
        gravityY = 0.0,
        friction = 0.6,
    },
    sprite = {
        filename = "assets/sprite/player.png",
        id = "player",
        animation = {
            {
                name = "down",
                row = 0,
                numFrames = 6
            },
            {
                name = "default",
                row = 1,
                numFrames = 6
            },
            {
                name = "up",
                row = 2,
                numFrames = 6
            },
            {
                name = "hitRight",
                row = 7,
                numFrames = 4
            },
            {
                name = "dash",
                row = 3,
                numFrames = 6
            }
        },
        width = 48,
        height = 48
    },
    collider = {
        type = "player",
        width = 15,
        height = 30,
        listener = "onCollide"
    },
    input = {
        listener = "onInput"
    },
    HP = 100
}

player = define(playerTable)

mapWidth = getMapWidth()
mapHeight = getMapHeight()
screenWidth = getScreenWidth()
screenHeight = getScreenHeight()

playerPosLabel = create()
-- ':' and ',' are displayed as squares because of the font!
playerPosLabel:addLabel(10, 10, "Player  Pos  "..math.floor(player:getX())..", "..math.floor(player:getY()), "arcade", 0, 102, 204, 255)
playerPosLabel:setLabelAlignment(0, 2)

print("Map Width: "..mapWidth.."\nMap Height: "..mapHeight)
print("Screen Width: "..screenWidth.."\nScreen Height: "..screenHeight)
print("Player ID: "..player:id())

testEntity = create()
testEntity:move(400, 400)
testEntity:scale(2, 2)
testEntity:addSprite("slime", "assets/sprite/slime.png", 32, 32)
testEntity:addAnimation("default", "0", "4")
testEntity:setAnimation("default")
testEntity:addCollider(48, 48)
testEntity:addPhysics(4.0, 0.3)
print("Slime ID: "..testEntity:id())

-- Testing deleting entity, needs to be deleted from global table as well
-- All this seem to do is remove from lua but __gc is not triggered in c++ and enity is not removed from EntityManager
--entities[testEntity:id()] = nil
--testEntity = nil


entityTable = {}
for i = 1, 100 do
    entityTable[i] = create()
    entityTable[i]:setType("enemy")
    entityTable[i]:move(100 + i * 100, 220 + i * 200)
    entityTable[i]:scale(1 * (i % 10), 1 * (i % 10))
    entityTable[i]:addSprite("player", "assets/sprite/player.png", 48, 48)
    entityTable[i]:addAnimation("default", 1, 6)
    entityTable[i]:setAnimation("default")
    entityTable[i]:addCollider(15, 30)
    entityTable[i]:addPhysics(2.0, 0.3)
end
