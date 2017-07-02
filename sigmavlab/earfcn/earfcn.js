// JavaScript Document
    
$('#e2fCalcBtn').on('click',function(){
    //$('#earfcn').html5Validate();
    var earfcn = $('#earfcn').val();
    var freqType = '';
    var resultHtml = '';
    var reg = /^\d+$/;
    if(!reg.test(earfcn) || earfcn < 0 || earfcn >= 46590){
      resultHtml += "<br><font color=\"red\">Invalid input, Integer Range: 0~46589.</font><br>";
      $('#e2fresult').html(resultHtml);
      return;
    }
    //$('#e2fresult').innerHTML
    //window.console.log($('#e2fresult'));
    var freqResult = 0;
    $.getJSON("earfcn.json",function(data){
      $.each(data,function(infoIndex,info){
        //window.console.log(info["dlEarfcnStart"]);
        if(earfcn >= info["dlEarfcnStart"] && earfcn < info["dlEarfcnEnd"]){
          if(info["bandId"] < 33){
            freqType = "DL";
          }
          else{
            freqType = "TDD";
          }
          freqResult = (earfcn - info["dlEarfcnStart"])*0.1 + info["dlFreqStart"];
          //window.console.log(freqResult);
        }
        else if(freqType !== "TDD" && earfcn >= info["ulEarfcnStart"] && earfcn < info["ulEarfcnEnd"]){
          freqType = "UL";
          freqResult = (earfcn - info["ulEarfcnStart"])*0.1 + info["ulFreqStart"];
        }
        else{
          return ;
        }
        resultHtml += "<li data-role=\"list-divider\">BandID:" + info["bandId"] + "</li>";
        resultHtml += "<li>FreqType:" + freqType + "</li>";
        resultHtml = resultHtml + "<li>Frequency:" + freqResult + "MHz</li>";
        resultHtml = resultHtml + "<li>DlFreqStart:" + info["dlFreqStart"] + "MHz</li>";
        resultHtml = resultHtml + "<li>DlFreqEnd:" + info["dlFreqEnd"] + "MHz</li>";
        resultHtml = resultHtml + "<li>DlEarfcnStart:" + info["dlEarfcnStart"] + "</li>";
        resultHtml = resultHtml + "<li>DlEarfcnEnd:" + info["dlEarfcnEnd"] + "</li>";
        if(freqType !== "TDD" ){
          resultHtml = resultHtml + "<li>UlFreqStart:" + info["ulFreqStart"] + "MHz</li>";
          resultHtml = resultHtml + "<li>UlFreqEnd:" + info["ulFreqEnd"] + "MHz</li>";
          resultHtml = resultHtml + "<li>UlEarfcnStart:" + info["ulEarfcnStart"] + "</li>";
          resultHtml = resultHtml + "<li>UlEarfcnEnd:" + info["ulEarfcnEnd"] + "</li>";
        }
        return false;
      });
      if(resultHtml === ''){
        resultHtml = "<br><font color=\"blue\"> Earfcn: " + earfcn + " Undefined!</font>";
      }
      //window.console.log(resultHtml);
      $('#e2fresult').html(resultHtml);
      $('#e2fresult').listview('refresh');
    });
    
    
});  

$('#f2eCalcBtn').on('click',function(){
    var freq = $('#freq').val();
    var freqType = '';
    var resultHtml = '';
    var reg = /^\d+(\.\d{1})?$/;
    //$('#e2fresult').innerHTML
    window.console.log(freq);
    if(!reg.test(freq) || freq <= 0 || freq >= 3800){
      resultHtml += "<br><font color=\"red\">Invalid input, Range: 0.1~3799.9 (step 0.1MHz) </font><br>";
      $('#f2eresult').html(resultHtml);
      return;
    }
    //window.console.log(freq);
    var earfcnResult = 0;
    $.getJSON("earfcn.json",function(data){
      $.each(data,function(infoIndex,info){
        //window.console.log(info["dlEarfcnStart"]);
        if(info["bandId"] < 33){
            freqType = "DL";
        }
        else{
            freqType = "TDD";
        }
        
        if(freq >= info["dlFreqStart"] && freq < info["dlFreqEnd"]){  
          earfcnResult = (freq - info["dlFreqStart"])*10 + info["dlEarfcnStart"];
          //window.console.log(earfcnResult);
        }
        else if(freqType !== "TDD" && freq >= info["ulFreqStart"] && freq < info["ulFreqEnd"]){
          freqType = "UL";
          earfcnResult = (freq - info["ulFreqStart"])*10 + info["ulEarfcnStart"];
        }
        else{
          return ;
        }
        resultHtml += "<li data-role=\"list-divider\">BandID:" + info["bandId"] + "</li>";
        resultHtml += "<li>FreqType:" + freqType + "</li>";
        resultHtml = resultHtml + "<li>Earfcn:" + earfcnResult + "</li>";
        resultHtml = resultHtml + "<li>DlFreqStart:" + info["dlFreqStart"] + "MHz</li>";
        resultHtml = resultHtml + "<li>DlFreqEnd:" + info["dlFreqEnd"] + "MHz</li>";
        resultHtml = resultHtml + "<li>DlEarfcnStart:" + info["dlEarfcnStart"] + "</li>";
        resultHtml = resultHtml + "<li>DlEarfcnEnd:" + info["dlEarfcnEnd"] + "</li>";
        if(freqType !== "TDD" ){
          resultHtml = resultHtml + "<li>UlFreqStart:" + info["ulFreqStart"] + "MHz</li>";
          resultHtml = resultHtml + "<li>UlFreqEnd:" + info["ulFreqEnd"] + "MHz</li>";
          resultHtml = resultHtml + "<li>UlEarfcnStart:" + info["ulEarfcnStart"] + "</li>";
          resultHtml = resultHtml + "<li>UlEarfcnEnd:" + info["ulEarfcnEnd"] + "</li>";
        }
        //return false;
      });
      if(resultHtml === ''){
        resultHtml = "<br><font color=\"blue\"> Frequency: " + freq + " Undefined!</font>";
      }
      window.console.log(resultHtml);
      $('#f2eresult').html(resultHtml);
      $('#f2eresult').listview('refresh');
    }); 
});  
