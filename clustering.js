var linkage = undefined;


function allowMergeWithClusterCardinality(clusterFrom, clusterTo, clusters, max) {
    let fromSize = clusters[clusterFrom].length;
    let intoSize = clusters[clusterTo].length;
    let val = fromSize + intoSize;
    return fromSize > 0 && intoSize > 0 && val <= max;
}

/**
 * Return the index of the clusters to be merged,
 * that is clusters having the minimal distance in the distance matrix
 * distances: [][] matrix of distances between clusters
 */
function findClustersToMerge(distances, clusters) {
    let mySet = new Set();
    
    let result = {
	"from" : -1,
	"into" : -1,
	"distance": Number.MAX_VALUE
    };
    let allowMoreThanMax = $('#input_allow_more_than_max').is(":checked");
    let mergeAllowed = false;
    while (!mergeAllowed) {
	// note: merge cluster with bigger index in cluster with smaller index
	for (let from = 1;  from < distances.length; ++from) {
	    for (let into = 0; into < from; ++into) {
		let d = distances[from][into];
		if (d < result.distance) {
		    result.distance = d;
		    result.from = from;
		    result.into = into;
		} 
	    }
	}
	if (!allowMoreThanMax) {
	    // methode stupide. Faire en sorte dans mergeMinLink que tout merge impossible pour question de cardinality soit noté avec une distance infinie.
	    mergeAllowed = allowMergeWithClusterCardinality(result.from, result.into, clusters, $('#input_num_max').val());
	    if (!mergeAllowed) {
	//	console.log("Abort merge " + result.from + " with " + result.into + ": cluster size exceed max");
		let obj = [result.from, result.into];
		if (mySet.has(obj)) {
		    break;
		} else {
		    mySet.add(obj);
		}
		// reset search for result.
		distances[result.from][result.into] = distances[result.into][result.from] = Number.MAX_VALUE;
		result.from = result.into = -1;
		result.distance = Number.MAX_VALUE;
	    }
	} else {
	    mergeAllowed = true;
	}
    }
    return result;
}

function mergeMinLink(clusters, distances, merge) {
    for (let i = 0;i < clusters[merge.from].length;++i) {
	for (let j = 0; j < clusters[merge.into].length; ++j) {
	    let elem1 = clusters[merge.from][i];
	    let elem2 = clusters[merge.into][j];
	    distances[elem1][elem2] = distances[elem2][elem1] = Number.MAX_VALUE;
	}
    }
    clusters[merge.into].push(...clusters[merge.from]);
    clusters[merge.from] = [];
    for (let x = 0;x < clusters.length; ++x) {
	if (x !== merge.into) {
	    // dist btw cluster x and y is the min dist of any elements in cluster x and y
	    let min = Number.MAX_VALUE;
	    for (let i = 0;i < clusters[x].length; ++i) {
		let outerElem = clusters[x][i];
		for (let j = 0;j < clusters[merge.into].length; ++j) {
		    let innerElem = clusters[merge.into][j];
		    if (distances[outerElem][innerElem] < min) {
			min = distances[outerElem][innerElem];
		    }
		}
	    }
	    for (let i = 0;i < clusters[x].length; ++i) {
		for (let j = 0; j < clusters[merge.into].length; ++j) {
		    let outerElem = clusters[x][i];
		    let innerElem = clusters[merge.into][j];
		    distances[outerElem][innerElem] = distances[innerElem][outerElem] = min;
		}
	    }
	}
    }
}


function keepMerging(clusters, lastMerge) {
    if (lastMerge !== null) {
	if (lastMerge.from === -1 || lastMerge.into === -1) {
	    return false;
	}
    }
    let res = 0;
    for (let c of clusters) {
	if (c.length > 0) {
	    res++;
	}
    }
    if (res > 1) {
	return true;
    } else {
	return false;
    }
}

/**
 * Perform the agglomerative clustering
 * clusters : [][] array of array of elements, initialy one element per cluster
 * distances : [][] array of distance objects (N² could be N*(N-1) if optim needed
 * mergeFunc: function actually performing the merge of clusters, updating distance matrix if needed
 *
 */
function agglomerativeHierarchicalClustering(clusters, distances, mergeFunc) {
    let linkage = [];
    let numElem = clusters.length;
    let numLeft = clusters.length;
    let merge = null;
    while (keepMerging(clusters, merge)) {
	//while(linkage.length < numElem-1) {
	//console.log(numLeft + " clusters left");
	merge = findClustersToMerge(distances, clusters);
	linkage.push(merge);
	mergeFunc(clusters, distances, merge);
	console.log("Merge " + merge.from + " into " + merge.into + " with dist:" + merge.distance);
    }
    return linkage;
}


function callClustering(min, max, w1, w2, w3)  {
    console.log("Calling callClustering with min=" + min
		+ " max=" + max
		+ " w1=" + w1
		+ " w2=" + w2
		+ " w3=" + w3);
    
    var clusters = [];
    for (let i = 0;i < flyweightDistances.numElem; ++i) {
	clusters.push([i]);
    }
    if (typeof linkage === 'undefined') {
	window.linkage = agglomerativeHierarchicalClustering(clusters,
							     flyweightDistances.distances,
							     mergeMinLink);

	let half = Math.round(flyweightDistances.numElem/2);
	analyseClustering( $("#input_num_min.valueAsNumber"),
			   $("#input_num_max.valueAsNumber"),
			   half);
	$("#nb_clusters").val(half);
    } else {
	alert("Clustering deja calculé");
    }
}

function pointToLayer (feature, latlng) {
    let fillColor = typeof feature.properties.clusterIndex === 'undefined' ? '#000' : cluster_colors[feature.properties.clusterIndex%cluster_colors.length]; 
    return L.circleMarker(latlng, {
	radius: 4,
	fillColor: fillColor,
	color: '#000',
	weight: 1,
	opacity: 1,
	fillOpacity: 0.8
    });
}

function onEachFeature(feature, layer) {
    var popupContent = "";
    if (feature.properties && feature.properties.firstName) {
	popupContent += "<p> #"
	    + (index)
	    + " "
	    + feature.properties.firstName
	    + " "
	    + feature.properties.lastName
	    + "</p>";
    }
    if (feature.properties && feature.properties.email) {
	popupContent += "<p>" + feature.properties.email + "</p>";
    }
    if (feature.properties.clusterIndex >= 0) {
	popupContent += "<p>Cluster: " + feature.properties.clusterIndex + "</p>";
    }
    // console.log(popupContent);
    layer.bindPopup(popupContent);
   
}



function replayLinkage(numClusters) {
    let clusters = [];
    for (let i = 0;i < flyweightDistances.numElem; ++i) {
	clusters.push([i]);
    }
    let upTo = linkage.length - numClusters + 1;
    for (let i = 0;i < upTo; ++i) {
	let merge = linkage[i];
	console.log("Replay merge " + i + ": " + merge.from + " into " + merge.into + " with dist " + merge.distance);
	clusters[merge.into].push(...clusters[merge.from]);
	clusters[merge.from] = [];
    }
    
    let cluster_count = 0;
    for (let c of clusters) {
	if (c.length > 0) {
	    for (let i = 0; i < c.length; ++i) {
		geoData.features[c[i]].properties.clusterIndex = cluster_count;
	    }
	    cluster_count++;
	}
    }
    return clusters;
}

function updateAnalysis(overMax, underMin, clusters) {
    let analysis = "";
    for (let c of overMax) {
	analysis += "<li> Cluster " + c + " has " + clusters[c].length + " elements (more than max)</li>"
    }
    for (let c of underMin) {
	if (clusters[c].length > 0) {
	analysis += "<li> Cluster " + c + " has " + clusters[c].length + " elements (less than min)</li>"
	}
    }
    
    $('#analysisResults').html(analysis);
}

function updateMap(clusters) {

    if (typeof window.centroidlines === 'undefined') {
	window.centroidlines = [];
    } else {
	for (layer of window.centroidlines) {
	    map.removeLayer(layer);
	}
    }
    map.removeLayer(layers);
    for (let c of clusters) {
	if (c.length > 0) {
	    let lat = 0;
	    let lon = 0;
	    for (let i = 0; i < c.length; ++i) {
		lat += geoData.features[c[i]].geometry.coordinates[1];
		lon += geoData.features[c[i]].geometry.coordinates[0];
	    }
	    lat /= c.length;
	    lon /= c.length;
	    
	    for (let i = 0; i < c.length; ++i) {
		let line = {
		    "type": "Feature",
		    "geometry": {
			"type" : "LineString",
			"coordinates": [
			    [lon, lat],
			    [geoData.features[c[i]].geometry.coordinates[0],
			     geoData.features[c[i]].geometry.coordinates[1]]
			]
		    },
		}
		let clusterIndex = geoData.features[c[i]].properties.clusterIndex;
		let fillColor = cluster_colors[clusterIndex%cluster_colors.length]; 
		window.centroidlines.push(L.geoJSON(line, {color:fillColor}).addTo(map));
	    }
	}
    }

    window.index = 0;
    window.layers = L.geoJSON(geoData, {
	pointToLayer: pointToLayer,
	onEachFeature: onEachFeature}).addTo(map);
}

function manualChange(elementId, clusterId) {
    console.log("Manualchange : elem " + elementId + " -> cluster " + clusterId);
    geoData.features[elementId-1].properties.clusterIndex = clusterId;
    // todo: modifier pour que replayLinkage modifie le clusterId directement et non les autres 
    updateMap(window.clusters);
    updateTable(window.clusters);
    
}

function updateTable(clusters) {
    $("#tablebody").empty();
    let cpt = 1;
    let tableContent = "";
     for (let c of clusters) {
	 if (c.length > 0) {
	     for(let i = 0;i < c.length; ++i) {
		 let feature = geoData.features[c[i]];
		 let clusterIndex = feature.properties.clusterIndex;
		 tableContent +=
		     "<tr>"
		     + "<td>" + cpt                          + "</td>"
		     + "<td>" + feature.properties.firstName + "</td>"
		     + "<td>" + feature.properties.lastName  + "</td>" 
		     + "<td>" + feature.properties.email     + "</td>"
		     + "<td><input id='cluster_input_elem_" + cpt + "' type='number' onchange=manualChange(" + cpt + ",cluster_input_elem_" + cpt + ".valueAsNumber)"+   " value='" + clusterIndex +"'/></td>"
		     + "</tr>";
		 cpt++;
	     }
	}
     }
    $("#datatable").html(tableContent);
    
}

function analyseClustering(min, max, numberOfClusters) {
    if (typeof linkage === 'undefined') {
	$('#analysisResults').html("Clustering not computed yet");
    } else {
	let numMerge = linkage.length;
	window.clusters = replayLinkage(numberOfClusters);

	let overMax = [];
	let underMin = [];
	for(let i = 0;i <clusters.length;++i) {
	    if (clusters[i].length > max) {
		overMax.push(i); 
	    }
	    if (clusters[i].length < min) {
		underMin.push(i);
	    }
	}
	updateAnalysis(overMax, underMin, clusters);
	updateMap(clusters);
	updateTable(clusters);
    }
}
