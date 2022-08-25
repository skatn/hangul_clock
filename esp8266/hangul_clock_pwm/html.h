#ifndef HTML_H
#define HTML_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Hangul Clock</title>
    <!-- <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous"> -->
    <style>
        
        :root {
            --bs-blue: #0d6efd;
            --bs-indigo: #6610f2;
            --bs-purple: #6f42c1;
            --bs-pink: #d63384;
            --bs-red: #dc3545;
            --bs-orange: #fd7e14;
            --bs-yellow: #ffc107;
            --bs-green: #198754;
            --bs-teal: #20c997;
            --bs-cyan: #0dcaf0;
            --bs-white: #fff;
            --bs-gray: #6c757d;
            --bs-gray-dark: #343a40;
            --bs-gray-100: #f8f9fa;
            --bs-gray-200: #e9ecef;
            --bs-gray-300: #dee2e6;
            --bs-gray-400: #ced4da;
            --bs-gray-500: #adb5bd;
            --bs-gray-600: #6c757d;
            --bs-gray-700: #495057;
            --bs-gray-800: #343a40;
            --bs-gray-900: #212529;
            --bs-primary: #0d6efd;
            --bs-secondary: #6c757d;
            --bs-success: #198754;
            --bs-info: #0dcaf0;
            --bs-warning: #ffc107;
            --bs-danger: #dc3545;
            --bs-light: #f8f9fa;
            --bs-dark: #212529;
            --bs-primary-rgb: 13, 110, 253;
            --bs-secondary-rgb: 108, 117, 125;
            --bs-success-rgb: 25, 135, 84;
            --bs-info-rgb: 13, 202, 240;
            --bs-warning-rgb: 255, 193, 7;
            --bs-danger-rgb: 220, 53, 69;
            --bs-light-rgb: 248, 249, 250;
            --bs-dark-rgb: 33, 37, 41;
            --bs-white-rgb: 255, 255, 255;
            --bs-black-rgb: 0, 0, 0;
            --bs-body-color-rgb: 33, 37, 41;
            --bs-body-bg-rgb: 255, 255, 255;
            --bs-font-sans-serif: system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial, "Noto Sans", "Liberation Sans", sans-serif, "Apple Color Emoji", "Segoe UI Emoji", "Segoe UI Symbol", "Noto Color Emoji";
            --bs-font-monospace: SFMono-Regular, Menlo, Monaco, Consolas, "Liberation Mono", "Courier New", monospace;
            --bs-gradient: linear-gradient(180deg, rgba(255, 255, 255, 0.15), rgba(255, 255, 255, 0));
            --bs-body-font-family: var(--bs-font-sans-serif);
            --bs-body-font-size: 1rem;
            --bs-body-font-weight: 400;
            --bs-body-line-height: 1.5;
            --bs-body-color: #212529;
            --bs-body-bg: #fff;
        }
        *,
        *::before,
        *::after {
            box-sizing: border-box;
        }

        body {
            margin: 0;
            font-family: var(--bs-body-font-family);
            font-size: var(--bs-body-font-size);
            font-weight: var(--bs-body-font-weight);
            line-height: var(--bs-body-line-height);
            color: var(--bs-body-color);
            text-align: var(--bs-body-text-align);
            background-color: var(--bs-body-bg);
            -webkit-text-size-adjust: 100%;
            -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
        }

        h6, .h6, h5, .h5, h4, .h4, h3, .h3, h2, .h2, h1, .h1 {
            margin-top: 0;
            margin-bottom: 0.5rem;
            font-weight: 500;
            line-height: 1.2;
        }
        h4, .h4 {
            font-size: calc(1.275rem + 0.3vw);
        }
        @media (min-width: 1200px) {
            h4, .h4 {
                font-size: 1.5rem;
            }
        }

        .container-md {
            width: 100%;
            padding-right: var(--bs-gutter-x, 0.75rem);
            padding-left: var(--bs-gutter-x, 0.75rem);
            margin-right: auto;
            margin-left: auto;
        }
        @media (min-width: 768px) {
            .container-md, .container-sm, .container {
                max-width: 720px;
            }
        }
        @media (min-width: 992px) {
            .container-lg, .container-md, .container-sm, .container {
                max-width: 960px;
            }
        }
        @media (min-width: 1200px) {
            .container-xl, .container-lg, .container-md, .container-sm, .container {
                max-width: 1140px;
            }
        }
        @media (min-width: 1400px) {
            .container-xxl, .container-xl, .container-lg, .container-md, .container-sm, .container {
                max-width: 1320px;
            }
        }

        .bg-light {
            --bs-bg-opacity: 1;
            background-color: rgba(var(--bs-light-rgb), var(--bs-bg-opacity)) !important;
        }

        .btn {
            display: inline-block;
            font-weight: 400;
            line-height: 1.5;
            color: #212529;
            text-align: center;
            text-decoration: none;
            vertical-align: middle;
            cursor: pointer;
            -webkit-user-select: none;
            -moz-user-select: none;
            user-select: none;
            background-color: transparent;
            border: 1px solid transparent;
            padding: 0.375rem 0.75rem;
            font-size: 1rem;
            border-radius: 0.25rem;
            transition: color 0.15s ease-in-out, background-color 0.15s ease-in-out, border-color 0.15s ease-in-out, box-shadow 0.15s ease-in-out;
        }
        @media (prefers-reduced-motion: reduce) {
            .btn {
                transition: none;
            }
        }
        .btn:hover {
            color: #212529;
        }
        .btn-check:focus + .btn, .btn:focus {
            outline: 0;
            box-shadow: 0 0 0 0.25rem rgba(13, 110, 253, 0.25);
        }
        .btn:disabled, .btn.disabled, fieldset:disabled .btn {
            pointer-events: none;
            opacity: 0.65;
        }

        .btn-primary {
            color: #fff;
            background-color: #0d6efd;
            border-color: #0d6efd;
        }
        .btn-primary:hover {
            color: #fff;
            background-color: #0b5ed7;
            border-color: #0a58ca;
        }

        @-webkit-keyframes spinner-border {
            to {
                transform: rotate(360deg) /* rtl:ignore */;
            }
        }
        @keyframes spinner-border {
            to {
                transform: rotate(360deg) /* rtl:ignore */;
            }
        }
        @media (prefers-reduced-motion: reduce) {
            .spinner-border {
                -webkit-animation-duration: 1.5s;
                animation-duration: 1.5s;
            }
        }
        .spinner-border {
            display: inline-block;
            width: 2rem;
            height: 2rem;
            vertical-align: -0.125em;
            border: 0.25em solid currentColor;
            border-right-color: transparent;
            border-radius: 50%;
            -webkit-animation: 0.75s linear infinite spinner-border;
            animation: 0.75s linear infinite spinner-border;
        }
        .spinner-border-sm {
            width: 1rem;
            height: 1rem;
            border-width: 0.2em;
        }

        .form-check {
            display: block;
            min-height: 1.5rem;
            padding-left: 1.5em;
            margin-bottom: 0.125rem;
        }
        .form-check-inline {
            display: inline-block;
            margin-right: 1rem;
        }
        .form-check .form-check-input {
            float: left;
            margin-left: -1.5em;
        }
        .form-check-input {
            width: 1em;
            height: 1em;
            margin-top: 0.25em;
            vertical-align: top;
            background-color: #fff;
            background-repeat: no-repeat;
            background-position: center;
            background-size: contain;
            border: 1px solid rgba(0, 0, 0, 0.25);
            -webkit-appearance: none;
            -moz-appearance: none;
            appearance: none;
            -webkit-print-color-adjust: exact;
            color-adjust: exact;
        }
        .form-check-input[type=radio] {
            border-radius: 50%;
        }
        .form-check-input:checked {
            background-color: #0d6efd;
            border-color: #0d6efd;
        }
        .form-check-input:checked[type=radio] {
            background-image: url("data:image/svg+xml,%3csvg xmlns='http://www.w3.org/2000/svg' viewBox='-4 -4 8 8'%3e%3ccircle r='2' fill='%23fff'/%3e%3c/svg%3e");
        }

        .form-control {
            display: block;
            width: 100%;
            padding: 0.375rem 0.75rem;
            font-size: 1rem;
            font-weight: 400;
            line-height: 1.5;
            color: #212529;
            background-color: #fff;
            background-clip: padding-box;
            border: 1px solid #ced4da;
            -webkit-appearance: none;
            -moz-appearance: none;
            appearance: none;
            border-radius: 0.25rem;
            transition: border-color 0.15s ease-in-out, box-shadow 0.15s ease-in-out;
        }
        @media (prefers-reduced-motion: reduce) {
            .form-control {
                transition: none;
            }
        }
        .form-control:focus {
            color: #212529;
            background-color: #fff;
            border-color: #86b7fe;
            outline: 0;
            box-shadow: 0 0 0 0.25rem rgba(13, 110, 253, 0.25);
        }
        .form-control::placeholder {
            color: #6c757d;
            opacity: 1;
        }

        .shadow-sm {
            box-shadow: 0 0.125rem 0.25rem rgba(0, 0, 0, 0.075) !important;
        }
        .bg-body {
            --bs-bg-opacity: 1;
            background-color: rgba(var(--bs-body-bg-rgb), var(--bs-bg-opacity)) !important;
        }
        .rounded {
            border-radius: 0.25rem !important;
        }

        .align-items-center {
            align-items: center !important;
        }

        .row {
            --bs-gutter-x: 1.5rem;
            --bs-gutter-y: 0;
            display: flex;
            flex-wrap: wrap;
            margin-top: calc(-1 * var(--bs-gutter-y));
            margin-right: calc(-0.5 * var(--bs-gutter-x));
            margin-left: calc(-0.5 * var(--bs-gutter-x));
        }
        .row > * {
            flex-shrink: 0;
            /* width: 100%; */
            max-width: 100%;
            padding-right: calc(var(--bs-gutter-x) * 0.5);
            padding-left: calc(var(--bs-gutter-x) * 0.5);
            margin-top: var(--bs-gutter-y);
        }
        .col-1 {
            flex: 0 0 auto;
            width: 8.33333333%;
        }
        .col-2 {
            flex: 0 0 auto;
            width: 16.66666667%;
        }

        .p-3 {
            padding: 1rem !important;
        }
        .mb-3 {
            margin-bottom: 1rem !important;
        }
        .ms-3 {
            margin-left: 1rem !important;
        }
    </style>
</head>
<body class="bg-light">
    <div class="container-md">
        <h1 class="display-4">한글시계 설정페이지</h1>
    
        <section class="shadow-sm p-3 mb-3 bg-body rounded">
            <h4>디스플레이 모드</h4>
            <span>현재 설정 모드 : </span><span id="display_mode">-</span>
            <br>
            <div class="form-check form-check-inline">
                <input class="form-check-input" type="radio" name="display_mode" value="0" id="display_standard">
                <label class="form-check-label" for="display_standard" >기본</label>
            </div>
            <div class="form-check form-check-inline">
                <input class="form-check-input" type="radio" name="display_mode" value="1" id="display_fade">
                <label class="form-check-label" for="display_fade">페이드
            </div>
            <button class="btn btn-primary" onclick="onClickSetDisplayMode();" custom-type="setting">
                적용
            </button>
        </section>
    
        <section class="shadow-sm p-3 mb-3 bg-body rounded">
            <h4>밝기</h4>
            <span>현재 설정 값 : </span><span id="brightness">-%</span>
            <br>
        
            <input type="range" id="slider" value="0" min="0" max="100" oninput="changeSlideValue(this.value)">
            <span id="slide_value">0%</span>
            
            <button class="btn btn-primary" onclick="onClickSliderDown();">
                -
            </button>
            <button class="btn btn-primary" onclick="onClickSliderUp();">
                +
            </button>
        
            
            <button class="btn btn-primary ms-3" onclick="onClickSetBrightness();" custom-type="setting">
                적용
            </button>
        </section>

        <section class="shadow-sm p-3 mb-3 bg-body rounded">
            <h4>타임존 설정</h4>
            <span>현재 설정 GMT : </span><span id="gmt-label">-</span>
            <br>
            
            <div class="row align-items-center">
                <span>GMT+</span>
                <input id="gmt-value" class="col-2 form-control" type="number" value="9" min="-11" max="12">
                <button class="btn btn-primary ms-3" onclick="onClickGMT();" custom-type="setting">
                    적용
                </button>
            </div>
            
            
        </section>
        
        <button class="btn btn-primary" onclick="onClickGetSettings();" custom-type="setting">
            세팅값 가져오기
        </button>
    </div>

    <script language = "javascript">
        const brightness = document.getElementById("brightness");
        const slider = document.getElementById("slider");
        const slideValue = document.getElementById("slide_value");
        const displayMode = document.getElementById("display_mode");
        const displayRadios = document.getElementsByName("display_mode");
        const gmtLabel = document.getElementById("gmt-label");
        const gmtValue = document.getElementById("gmt-value");



        const server = `http://${window.location.host}`;

        function onClickGetSettings(){
            request(`${server}/get_settings`)
            .then((result)=>{
                console.log(result);
                const split = result.split(",");
                const display_mode = split[0];
                const brightness = split[1];
                const gmt = split[2];

                updateDisplay(Number(display_mode));
                updateBrightness(brightness);
                updateGmtLabel(gmt);
            })
            .catch((error)=>{
                console.log(error);
                alert("세팅값 가져오기에 실패하였습니다.");
            });
        }

        function onClickSetDisplayMode(){
            let displayMode;
            for(let i=0; i<displayRadios.length; i++){
                if(displayRadios[i].checked === true){
                    displayMode = displayRadios[i].value;
                    break;
                }
            }

            if(displayMode === undefined){
                alert("모드를 선택해 주세요");
                return;
            }

            request(`${server}/set_display_mode?mode=${displayMode}`)
            .then((result)=>{
                if(result==="SUCCESS"){
                    updateDisplay(Number(displayMode));
                }
            })
            .catch((error)=>{
                console.log(error);
                alert("디스플레이 모드 설정에 실패하였습니다.");
            });
        }

        function onClickSetBrightness(){
            const value = slider.value;
            request(`${server}/set_brightness?brightness=${value}`)
            .then((result)=>{
                if(result === "SUCCESS"){
                    updateBrightness(value);
                }
            })
            .catch((error)=>{
                console.log(error);
                alert("밝기 설정에 실패하였습니다.");
            });
        }
        
        function onClickGMT(){
            const value = gmtValue.value;
            console.log(value);

            if(value < -11 || value > 12){
                alert("-11 ~ 12 사이의 값을 입력해 주세요.")
                return;
            }

            request(`${server}/set_gmt?gmt=${value}`)
            .then((result)=>{
                if(result==="SUCCESS"){
                    updateGmtLabel(value);
                }
            })
            .catch((error)=>{
                console.log(error);
                alert("GMT 설정에 실패하였습니다.");
            });
        }

        function onClickSliderUp(){
            slider.value = parseInt(slider.value) + 1;
            changeSlideValue(slider.value);
            console.log(slider.value);
        }
        function onClickSliderDown(){
            slider.value = parseInt(slider.value) - 1;
            changeSlideValue(slider.value);
            console.log(slider.value);
        }

        function loadingStart(){
            const buttons = document.getElementsByTagName("button");
            for(const button of buttons){
                if(button.getAttribute("custom-type") !== "setting") continue;

                button.disabled = true;
                const spinner = document.createElement("span");
                spinner.setAttribute("class", "spinner-border spinner-border-sm");
                spinner.setAttribute("role", "status");
                spinner.setAttribute("aria-hidden", "true");

                button.appendChild(spinner);
            }
        }

        function loadingEnd(){
            const buttons = document.getElementsByTagName("button");
            for(const button of buttons){
                if(button.getAttribute("custom-type") !== "setting") continue;
                
                button.disabled = false;
                const spinner = button.childNodes[1]
                button.removeChild(spinner);
            }
        }

        function request(uri){
            loadingStart();
            return new Promise((resolve, reject)=>{
                var xhttp = new XMLHttpRequest();
                
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        console.log(xhttp.responseText);
                        resolve(xhttp.responseText);
                        loadingEnd();
                    }
                };
                xhttp.onerror = function () {
                    loadingEnd();
                    reject("ERROR");
                };
        
                xhttp.open("GET", uri, true);
                xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
                xhttp.send();
            });
        }


        function updateDisplay(mode){
            if(mode === 0){
                displayRadios[0].checked = true;
                displayMode.innerText = "기본";
            }
            else{
                displayRadios[1].checked = true;
                displayMode.innerText = "페이드";
            }
        }

        function updateBrightness(value){
            slider.value = value;
            brightness.innerText = value;
            slideValue.innerText = value.toString() + "%";
        }

        function updateGmtLabel(gmt){
            gmtLabel.innerText = gmt;
        }

        function changeSlideValue(value){
            slideValue.innerText = value.toString() + "%";
        }
    </script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p" crossorigin="anonymous"></script>
</body>
</html>
)rawliteral";

#endif 
