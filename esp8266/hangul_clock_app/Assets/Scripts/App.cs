using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Networking;


public class App : MonoBehaviour
{
    readonly string uri = "http://192.168.4.1/get";
    delegate void fp(string data);
    fp webRequestResult;
    int displayMode, brightness;
    float time;

    public Slider brightnessBar;
    public Dropdown dropdown;
    public Text textDescription, textSync;

    // Start is called before the first frame update
    void Start()
    {
        brightnessBar.onValueChanged.AddListener(OnBrightnessBarChanged);
    }

    // Update is called once per frame
    void Update()
    {
    }

    public void GetSettings()
    {
        webRequestResult = new fp(GetSettingsResult);
        StartCoroutine(WebRequest("get_settings=0"));
    }

    public void SetDisplayMode()
    {
        webRequestResult = new fp(SetDisplayModeResult);
        StartCoroutine(WebRequest($"set_display_mode={dropdown.value}"));
    }

    void Nothing(string data)
    {

    }

    void GetSettingsResult(string data)
    {
        string[] datas = data.Split(',');
        displayMode = int.Parse(datas[0]);              //0: standard / 1: fade
        brightness = int.Parse(datas[1]);

        dropdown.value = displayMode;
        brightnessBar.value = brightness;
        textSync.text = displayMode.ToString() + "," + brightness.ToString();
    }

    void SetDisplayModeResult(string data)
    {
        if (data.Contains("success"))
        {
            displayMode = dropdown.value;
        }
    }

    void OnBrightnessBarChanged(float value)
    {
        webRequestResult = new fp(Nothing);
        StartCoroutine(WebRequest($"set_brightness={brightnessBar.value}"));
    }

    IEnumerator WebRequest(string data)
    {
        using(UnityWebRequest request = UnityWebRequest.Get(uri+"?"+data))
        {
            yield return request.SendWebRequest();

            if (request.isNetworkError)
            {
                Debug.Log(request.error);
            }
            else
            {
                Debug.Log($"Get: {request.downloadHandler.text}");
                webRequestResult(request.downloadHandler.text);
                textDescription.text = request.downloadHandler.text;
            }
        }
    }
}
