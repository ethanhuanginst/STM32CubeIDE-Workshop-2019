# STM32CubeIDE 工作坊
![](docs/imgs/ST14179_STM32CubeIDE.png)

日期: 2019.05.22 (三)

時間: 09:30 - 17:00

地點: 台北市內湖區基湖路10巷50號2樓[[Google map]](https://goo.gl/maps/c8vGBHJDmh5RcEpR7)

開發板: [32F746GDISCOVERY](https://www.st.com/en/evaluation-tools/32f746gdiscovery.html)



## 事前準備

1. 由以下網址下載並安裝STM32CubeIDE：

   https://www.st.com/en/development-tools/stm32cubeide.html

   ![](docs/imgs/Download_STM32CubeIDE.png)

   * 下載STM32軟體時需要登入myST（如下所示）。若沒有myST帳號，請先註冊。
    ![](docs/imgs/ST_Registration.png)
     
   * 依文件 [STM32CubeIDE installation guide UM2563](https://www.st.com/resource/en/user_manual/dm00603964.pdf)所述，目前STM32CubeIDE**只支援64-bit OS**。

      

2. 由以下網址下載並安裝STM32Programmer：

   https://www.st.com/en/development-tools/stm32cubeprog.html

   ![](docs/imgs/Download_STM32CubeProgrammer.png)

   * 需要登入myST。

     

3. 由以下網址下載STM32CubeF7 MCU package：

   https://www.st.com/en/embedded-software/stm32cubef7.html

   ![](docs/imgs/Download_STM32CubeF7.png)

   * 需要登入myST。

     

4. 受公司proxy管理的筆電需要額外設定「網路連線」才能正常使用STM32CubeIDE。 

   * 開啟STM32CubeIDE
   
   * 使用預設的workspace
   
     ![](/docs/imgs/STM32CubeIDE_Launcher.png)
   
   * 出現以下的對話框時建議選取「No」：

     .cproject file extension:
     
     ![](/docs/imgs/STM32CubeIDE_Override_File_Extension.png)
     
     .project file extension:
     
     ![](/docs/imgs/STM32CubeIDE_Override_File_Extension_2.png)
   
   * 選取Window --> Preferences（如下所示）
     ![](docs/imgs/STM32CubeIDE_Proxy_1.png)
   * 選取General --> Network Connection（如下所示），並依貴公司所提供的proxy設定填入底下的欄位（一般主要是HTTP及HTTPS）：
     ![](docs/imgs/STM32CubeIDE_Proxy_2.png)
   
   * 「網路連線」設定完成之後，可以到STM32Cube --> Firmware Updater（如下所示）按「Check Connection」按鈕來檢查網路連線狀態，若出現綠色打勾的符號代表連線正常，若出現紅色交叉符號代表連線失敗，請重新確認前一步驟的「網路連線」設定是否正確。
   
     ![](docs/imgs/STM32CubeIDE_Proxy_3.png)



5. 為了在離線時能夠讀取從Github下載的說明檔README.md，建議下載Markdown閱覽器/編輯器，如[Typora](https://typora.io/)。