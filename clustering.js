var linkage = undefined;

/**
 * Return the index of the clusters to be merged,
 * that is clusters having the minimal distance in the distance matrix
 * distances: [][] matrix of distances between clusters
 */
function findClustersToMerge(distances) {
    let min = Number.MAX_VALUE;
    let result = {
	"from" : -1,
	"into" : -1
    };

    // note: merge cluster with bigger index in cluster with smaller index
    for (let from = 1;  from < distances.length; ++from) {
	for (let into = 0; into < from; ++into) {
	    if (distances[from][into] < min) {
		min = distances[from][into];
		result.from = from;
		result.into = into;
	    }
	}
    }
    result.distance = min;
    return result;
}

function mergeMinLink(clusters, distances, merge) {
    for (var elem1 of clusters[merge.from]) {
	for (var elem2 of clusters[merge.into]) {
	    distances[elem1][elem2] = Number.MAX_VALUE;
	}
    }
    clusters[merge.into].push(...clusters[merge.from]);
    clusters[merge.from] = [];
    for (let x = 0;x < clusters.length; ++x) {
	if (x !== merge.into) {
	    // dist btw cluster x and y is the min dist of any elements in cluster x and y
	    let min = Number.MAX_VALUE;
	    for (let elemX of clusters[x]) {
		for (let elemInto of clusters[merge.into]) {
		    if (distances[elemX][elemInto] < min) {
			min = distances[elemX][elemInto];
		    }
		}
	    }
	    for (let elemX of clusters[x]) {
		for (let elemInto of clusters[merge.into]) {
		    distances[elemX][elemInto] = distances[elemInto][elemX] = min;
		}
	    }
	}
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
    while (linkage.length !== numElem - 1) {
	let merge = findClustersToMerge(distances);
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

    $('#clusteringParameters')
	.ajaxForm({
	    url : '#',
	    dataType : 'json',
	    success : function(response) {
		window.linkage = response;
	    }
	});

    
    // var clusters = [];
    // for (let i = 0;i < flyweightDistances.numElem; ++i) {
    // 	clusters.push([i]);
    // }
    // if (typeof window.linkage === 'undefined') {
    // 	window.linkage = agglomerativeHierarchicalClustering(clusters,
    // 						      flyweightDistances.distances,
    // 						      mergeMinLink);
    // } else {
    // 	console.alert("Linkage deja calculé");
    // }
    
}
