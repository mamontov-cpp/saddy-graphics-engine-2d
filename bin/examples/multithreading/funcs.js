function makeRectPlatform(x, y, w, h)
{
    if ((w > 0) && (h > 0)) {
        var i = 0, j, px, py;
        for (i = 0; i < w; i++) {
            px = x + i;
            py = (h - 1) * 16
            E.scenenodes._addSprite2D("1", "/tiles_list/tileBrown_24ng", "tile_" + i, r2d(px * 16, py, (px + 1) * 16, py + 16), aclr(255, 255, 255));
        }
        /*for (j = h - 1; j > -1; j++)
        {
            for (i = 0; i < w; i++) {
                px = x + i;
                py = (j - 1) * 16
                E.scenenodes._addSprite2D("1",  "/tiles_list/tileBrown_18ng", "tile_" + i, r2d(px * 16, py, (px + 1) * 16 + 16, py + 16), aclr(255, 255, 255));
            }
        }*/
    }
}

makeRectPlatform(0, 2, 40, 2);
