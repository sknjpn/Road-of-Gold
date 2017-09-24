
# Lua関数リスト

[もし、動作が分からない場合はこちらから気軽に質問してください！](https://github.com/SKN-JP/Road-of-Gold/issues)
##  print(anyData)
引数のデータをウィンドウ上に出力します  
予期せぬバグ対策用などデバッグ用に使うといいでしょう  
### sample
```lua
a = 412
print("文字列と数値を出力する a = "..a)
print(true) --boolean型の出力もできます
```
   

##  output(anyData)
引数のデータをログ上に出力(.logファイルに出力)します  
予期せぬバグ対策用などデバッグ用に使うといいでしょう  
### sample
```lua
    a = 412
    print("文字列と数値を出力する a = "..a)
    print(true) --boolean型の出力もできます
```

##  addScuttle(title, document, button)
引数のデータを利用してイベントウィンドウを作ります  
titleはタイトルとして使うテキストを、  
documentは本文として使うテキストを、  
buttonはボタンとして使うテキストを  
それぞれ代入してください。  
### sample
```lua
    --localはローカル変数として宣言する時に使います
    local title = "Luaからのイベント"..year.."年"..month.."月"..day.."日"
    local document = "本文です"
    local button = "これがボタンです"
    addScuttle(title, document, button) --直接入力してもよいですが、見づらくなるので変数にいったん代入するのをお勧めします
```

##  getMapPath()
マップファイルへのパスを取得します  
例:assets/map/europe/  
これはマップ名が変わり、ファイルパスが変更されても問題なく動作するために活用されます。  
特にマップフォルダ内のアセットを読み込む際に重要です  
### sample
```lua
    mapPath = getMapPath()  --マップファイルのパスを取得
    print(mapPath)  --ウィンドウ上に表示
    setBGM(mapPath.."music.mp3")--マップファイル内にあるmusic.mp3を再生します
```

##  setBGM(filePath, volume)
filePathの音楽ファイルをRoad of GoldのBGMとして再生します。  
playAudio()との違いはループすること、複数のBGMは再生されないことです。  
すでに再生されている場合、上書きして再生されます。  
volumeは音量で、0.0～1.0の間で指定します。省略時は1.0となります。  
返り値はboolean型で再生に失敗するとfalseを返します  
### sample1
```lua
    setBGM("assets/BGM/music.mp3")  --music.mp3を再生します
```
### sample2
```lua
    year, month, day = getDate();   --日付の取得
    if bgmFlag == false and year == 150 and month == 5 and day == 1 then --日付を条件にして再生
    setBGM("assets/BGM/music.mp3")  --music.mp3を再生します
    bgmFlag = true  --フラグを立てて多重に読み込まれることを防止します
    end
```
### sample3
```lua
    if not setBGM("assets/BGM/music.mp3") then  --music.mp3を再生します
    print("再生に失敗しました")  --再生に失敗した場合、falseが返されます
    end
```

##  playAudio(filePath)
filePathの音声ファイルを再生します  
setBGM()との違いは複数のAudioを再生可能なことです。  
すでに再生されていて呼び出されても止まることはありません。  
主に効果音として使われることを想定しています。  
返り値はboolean型で再生に失敗するとfalseを返します  
### sample1
```lua
    playAudio("assets/sound/s01.mp3")  --s01.mp3を再生します
```
### sample2
```lua
    if not playAudio("assets/sound/s01.mp3") then   --s01.mp3を再生します
    print("再生に失敗しました")  --再生に失敗した場合、falseが返されます
    end
```



##  getDate()
日付を返します。  
### sample
```lua
    year, month, day = getDate();   --日付の取得
    print("今は"..year.."年"..month.."月"..day.."日です")
```
