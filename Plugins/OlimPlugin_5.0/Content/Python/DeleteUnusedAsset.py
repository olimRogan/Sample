import unreal
​
workingPath = "/Game/Unique/"
​
@unreal.uclass()
class GetEditorAssetLibrary(unreal.EditorAssetLibrary):
    pass
editorAssetLib = GetEditorAssetLibrary()
​
allAssets = editorAssetLib.list_assets(workingPath, True, False)
​
processingAssetPath = ""
escape = '/3DResources/'
currentIndex = 1
allAssetsCount = len(allAssets)
deletedList = []
if ( allAssetsCount > 0):
    with unreal.ScopedSlowTask(allAssetsCount, processingAssetPath) as slowTask:
        slowTask.make_dialog(True)
        for asset in allAssets:
            if slowTask.should_cancel():         # True if the user has pressed Cancel in the UI
                break
            slowTask.enter_progress_frame(1, processingAssetPath)
            #print ('current index {0} remain {1} total {2}'.format(currentIndex, allAssetsCount - currentIndex, allAssetsCount))
            currentIndex += 1
            processingAssetPath = asset
            index = processingAssetPath.find(escape)            
            if (index == -1):
                continue
            deps = editorAssetLib.find_package_referencers_for_asset(asset, False)
            if (len(deps) <= 0):
                #print ">>> Deleting >>> %s" % asset
                editorAssetLib.delete_asset(asset)
                deletedList.append(asset)
​
    print('============================================================')
    print('============================================================')
    print('============================================================')
    print('============================================================')
    print('============================================================')
    print('============================================================')
    print('====deleted file list=======================================')
    print('====please check deleted file list before push==============')
    for deletedAsset in deletedList:
        print(deletedAsset)
    print('============================================================')
    print('============================================================')
    print('============================================================')
    print('============================================================')
    print('============================================================')
    print('============================================================')