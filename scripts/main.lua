-- TODO: need to import global entity table and some helper methods to all states to make scripting easier
background = Entity.create()
background:addSprite("clouds2", "", 800, 600)
background:addBackground(1)

title = Entity.create()
title:addLabel("VIGILANT ENGINE", "arcade", 0, 10, 0, 102, 204, 255)
title:setAlignment(1, 0)

-- FIXME: Currently listener will be triggered, but thisId is useless,
-- because I couldn't get entities table be shared across multiple scripts
function onPlayClick(thisId)
    playSound("button")
    changeState("PLAY")
end

function onQuit(thisId)
    quit()
end

playButton = Entity:create()
playButton:addButton("playbutton", 300, 100, 0, -75)
playButton:setAlignment(1, 1)
playButton:setButtonListener("onPlayClick")

quitButton = Entity:create()
quitButton:addButton("exitbutton", 300, 100, 0, 75)
quitButton:setAlignment(1, 1)
quitButton:setButtonListener("onQuit")
