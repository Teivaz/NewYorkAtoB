try:
	from PIL import ImageFile
	import PIL
except ImportError:
	raise ImportError("This script requires module `pillow`\nTry `sudo pip install pillow`")
import json
	
# should move this to config
MAP_NAME = "../RawResources/new_york_map.jpg"
MAP_COORDINATES = (0, 0, 3398, 4842)
MAX_LOD = 4

# convertor config
TILE_SIZE = (512, 512)
RESOURCES_FOLDER = "../Resources/"
DEST_FOLDER = "map/"
CONFIG_NAME = "map.json"

def nameForTile(left, top, lod):
	A = left / TILE_SIZE[0]
	B = top / TILE_SIZE[1]
	C = lod
	return "{0}_{1}_{2}.png".format(A, B, C)


def TileImage(image, mapCoordinates, lod, config):

	# Scale down the image to the corresponding LOD
	lodMultiplier = pow(2, lod)
	imageSize = image.size
	imageSize = (imageSize[0]/lodMultiplier, imageSize[1]/lodMultiplier)
	image = image.resize(imageSize, PIL.Image.BICUBIC)

	# Region always has (0,0) as top left corner
	regionSize = [0, 0, TILE_SIZE[0], TILE_SIZE[1]]
	mapOffsetX = mapCoordinates[0]/lodMultiplier
	mapOffsetY = mapCoordinates[1]/lodMultiplier
	mapSizeX = mapCoordinates[2]/lodMultiplier - mapOffsetX
	mapSizeY = mapCoordinates[3]/lodMultiplier - mapOffsetY

	while regionSize[1] < mapSizeY: #till the end of Y axis
		regionSize[0] = 0
		regionSize[2] = TILE_SIZE[0]
		while regionSize[0] < mapSizeX: #till the end of X axis
			subimageRegion = (
				regionSize[0], 
				imageSize[1]-regionSize[3], 
				regionSize[2], 
				imageSize[1]-regionSize[1])
			print(subimageRegion)
			subimage = image.crop(subimageRegion)
			regionIndexX = regionSize[0]+mapOffsetX
			regionIndexY = regionSize[1]+mapOffsetY
			subimageName = nameForTile(regionIndexX, regionIndexY, lod)
			configItem = {
				"lod": lod,
				"bounds": [	regionIndexX*lodMultiplier,
							regionIndexY*lodMultiplier,
							(regionSize[2]+mapOffsetX)*lodMultiplier,
							(regionSize[3]+mapOffsetY)*lodMultiplier],
				"name": DEST_FOLDER + subimageName}
			config.append(configItem)
			subimage.save(RESOURCES_FOLDER + DEST_FOLDER + subimageName)
			regionSize[0] += TILE_SIZE[0]
			regionSize[2] += TILE_SIZE[0]
		regionSize[1] += TILE_SIZE[1]
		regionSize[3] += TILE_SIZE[1]
	
	mapWidth = mapCoordinates


def convert(mapFileName, mapCoordinates, maxLod):
	fp = open(mapFileName, "rb")
	p = ImageFile.Parser()
	while 1:
		s = fp.read(1024)
		if not s:
			break
		p.feed(s)
	fp.close()
	im = p.close()

	im = im.convert("RGBA")
	
	lodConfig = []
	for lod in range(0, maxLod):
		TileImage(im, mapCoordinates, lod, lodConfig)

	config = {
		"map":
		{
			"name": mapFileName,
			"bounds": mapCoordinates,
			"maxLod": maxLod,
			"tileWidth": TILE_SIZE[0],
			"tileHeight": TILE_SIZE[1]
		},
		"tiles": lodConfig
	}

	cf = open(RESOURCES_FOLDER+DEST_FOLDER+CONFIG_NAME, "w")
	cf.write(json.dumps(config, indent=4, sort_keys=True))
	cf.close()

convert(MAP_NAME, MAP_COORDINATES, MAX_LOD)
