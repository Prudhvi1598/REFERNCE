import appCtxSvc from 'js/appCtxService';
import app from 'app';

var exports = {};


let getRelationTypes = function(data) {
    var RelationTypes = appCtxSvc.ctx.preferences.ItemRevision_shown_relations;
	var size=RelationTypes.length;
	console.log(size);
	var relationArray=[];
    for( let i=0;i<size;i++)
	{
	relationArray.push({propDisplayValue:RelationTypes[i],propInternalValue:RelationTypes[i]});
	}
	console.log(relationArray);
	return relationArray;
	
};

export default exports={
	getRelationTypes
};

app.factory( 'PropertyUpdateService', () => exports );

	
	
	