--[[
    注意事項
    ・日数当たりの呼ばれる回数は倍速設定によって異なる
    ・倍速は1日/フレームを超えることはない。例：11日から13日に飛ぶ倍速は行われない
    ・イベントウィンドウはScuttleと表現される
    ・文法チェックは一切されないため、バグに注意が必要
]]

--この段階では都市展開がされていないため、本体に影響を与えるコマンドは推奨されません。
mapPath = getMapPath()

--最初に一回だけ呼ばれます
function init()
    setBGM(mapPath.."bgm/午後の寄り道.mp3")   --BGMの設定
end

--イベントのメイン関数、毎フレーム呼ばれる
function update()
    clearPrint()    --ウィンドウ上の表示をクリアする
    year, month, day = getDate()   --毎フレームごとに更新するためにupdate()内に入れましょう
    print(month.."月"..day.."日")

    if not flag1 and year == 1000 and month == 5 and day == 12 then
        local title = "Luaからのイベント"
        local document = "本文です\n"..mapPath..year.."年"..month.."月"..day.."日\n東京にカカオを追加します"
        local button = "これがボタンです"
        addScuttle(title, document, button) --直接入力してもよいですが、見づらくなるので変数にいったん代入するのをお勧めします
        playAudio("assets/sound/s1.mp3")
        setEnergy("カカオ", "東京", 142) --Energyの登録を行います
        setBGM(mapPath.."bgm/攻城戦.mp3")   --BGMの設定

        flag1 = true --フラグを作ることで多重にイベントが起こるのを防ぎます
    end

    if month == 1 and day == 1 then
        if not flag2 then
            local numProduct = getNumProductionPerDay("ショコラ", "金沢")    --Item名とEnergy名は異なることに注意
            local title = "ショコラ生産統計"
            local document = "金沢におけるショコラ生産の統計が発表されました"
            document = document.."\n生産量"..numProduct
            local button = "了承"

            addScuttle(title, document, button) --直接入力してもよいですが、見づらくなるので変数にいったん代入するのをお勧めします
            playAudio("assets/sound/s1.mp3")
            flag2 = true
        end
    else
        flag2 = false
    end
end