// Copyright (c) 2019 Siemens

/**
 * @module js/staticLovData
 */
import app from 'app';
import appCtxSvc from 'js/appCtxService';
var exports = {};
var tokenArray=[];



 
export let search = function(decision)
{
	console.log(decision);
	var outputVals=[];
    if(decision===tokenArray[1]){
	console.log(tokenArray.length);
		
	for(let j=1;j<tokenArray.length;j++)
	{
	outputVals.push({propDisplayValue:tokenArray[j] , propDisplayDescription: tokenArray[j], propInternalValue: tokenArray[j]});
	console.log("loop entered");
	}
	console.log("Entered to ECN");
	}
	else
	{
		console.log(tokenArray.length);
		console.log(tokenArray);
		for(let j=1;j<tokenArray.length;j++)
		{
			outputVals.push({propDisplayValue:tokenArray[j] , propDisplayDescription: tokenArray[j], propInternalValue: tokenArray[j]});
		}
	console.log("Entered to ECR");
	}
	return outputVals;
};

export let rejectSuggestions = function( selected, suggestion ) {
    var valid = true;
    var message = '';

    if( suggestion ) {
        valid = false;
        message = 'The value ' + suggestion + ' is not a valid value.';
    }

    return {
        valid: valid,
        message: message
    };
};

export let switchListBox = function( selected0,selected,prop ){
	var ctx = appCtxSvc.ctx;
	var concatvalue=selected0.concat("~");
	selected=concatvalue.concat(selected);
	console.log(selected);
	if (ctx.preferences && ctx.preferences.Custom_Create_ECR_ECN){
		
	console.log( ctx.preferences.Custom_Create_ECR_ECN.length );
		
	let x= ctx.preferences.Custom_Create_ECR_ECN.length;
	console.log(x);
	for(let i=0;i<x;i++)
	{
		console.log(ctx.preferences.Custom_Create_ECR_ECN[i]);
		tokenArray=ctx.preferences.Custom_Create_ECR_ECN[i].split(";");
		//let y=ctx.preferences.Custom_Create_ECR_ECN[i].indexOf(";");
		//var substr=ctx.preferences.Custom_Create_ECR_ECN[i].substring(0,y);
		//console.log(substr);
		//var substr2=ctx.preferences.Custom_Create_ECR_ECN[i].substring(y+1,ctx.preferences.Custom_Create_ECR_ECN[i].length);
		console.log(tokenArray[0]);
		
		
		if(tokenArray[0]===selected)
		{
			console.log(tokenArray[1]);
			prop.dbValue =tokenArray[1];
			
			prop.uiValue = tokenArray[1];
			break;
		}
		
		
		
	}
	}
    
};


export default exports = {
    switchListBox,
	rejectSuggestions,
	search
};
/**
 * @memberof NgServices
 * @member createChangeService
 */
app.factory( 'staticLovData', () => exports );
