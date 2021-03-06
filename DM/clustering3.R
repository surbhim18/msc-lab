#Generating random numbers
num <- sample(50:100, 5, replace=TRUE)

library(MASS)
#First distribution - Gaussian
u1 <- c(-7,0)
sigma1 <- matrix(c(10,2,2,2),2,2)
d1 <- mvrnorm(n = num[1], u1 , sigma1)


#Second distribution - Gaussian
u2 <- c(12,12)
sigma2 <- matrix(c(2,3,3,10),2,2)
d2 <- mvrnorm(n = num[2], u2 , sigma2)


#Third distribution - Gaussian
u3 <- c(5,10)
sigma3 <- matrix(c(10,4,5,20),2,2)
d3 <- mvrnorm(n = num[3], u3 , sigma3)


#library downloaded from https://cran.r-project.org/web/packages/LaplacesDemon/index.html
library(LaplacesDemon)
#Fourth distribution - Exponential
u4 <- c(-10,15)
sigma4 <- matrix(c(10,5,5,20),2,2)
d4 <- rmvpe(num[4], u4, sigma4, kappa=1)


#Fifth distribution - Uniform
xmin <- min(d1[,1],d2[,1],d3[,1],d4[,1])
xmax <- max(d1[,1],d2[,1],d3[,1],d4[,1])
ymin <- min(d1[,2],d2[,2],d3[,2],d4[,2])
ymax <- max(d1[,2],d2[,2],d3[,2],d4[,2])
d5 <- matrix(c(runif(num[5],xmin,xmax),runif(num[5],ymin,ymax)),num[5],2)


#assigning class labels to points
d1 <- cbind(d1,1)
d2 <- cbind(d2,2)
d3 <- cbind(d3,3)
d4 <- cbind(d4,4)
d5 <- cbind(d5,5)

#combining points
dataset <- as.matrix(rbind(d1, d2, d3, d4,d5))

#find min y from all plots, max y from all plots
xlim = c(xmin,xmax)
ylim = c(ymin,ymax)

#plotting all points in one graph with different colors
color <- c("blue","red","green","yellow","black")

plot(dataset[,1], dataset[,2], xlab="X", ylab="Y", col = ifelse(dataset[,3] == 1, color[1], ifelse(dataset[,3] == 2, color[2], ifelse(dataset[,3] == 3, color[3],ifelse(dataset[,3] == 4, color[4],color[5])))))

legend(xlim[2]-6, ylim[2]-1, legend=c("G1","G2","G3","Exp","Uni") , cex=0.8, pch = 1, col=color, title="Data points")

#CLUSTERING
require(graphics)

#dataset without class labels
cdataset = dataset[,-3]

#K-MEANS
#kmeans(x, centers, iter.max = 10)
clk <- kmeans(cdataset, 5, 10)
plot(cdataset, col = clk$cluster, pch = clk$cluster, xlab="X", ylab="Y")
points(clk$centers, col = "black" , pch = 8, cex = 1)
title("K-means")
legend(xlim[2]-6, ylim[2]-1, legend=c("1","2","3","4","5") , cex=0.8, col = unique(clk$cluster), pch = unique(clk$cluster), title="Clusters")
text(xlim[2]-5, ylim[1]+3, "SSE = \nPurity = ", cex=0.8)

#HIERARCHICHAL CLUSTERING
hc <- hclust(dist(cdataset), "ward.D")
clh <- cutree(as.hclust(hc), k = 5)
plot(cdataset, col = clh, pch = clh, xlab="X", ylab="Y")
title("Heirarchial Clustering")
legend(xlim[2]-6, ylim[2]-1, legend=c("1","2","3","4","5") , cex=0.8, col = unique(clh), pch = unique(clh), title="Clusters")
text(xlim[2]-5, ylim[1]+3, "SSE = \nPurity = ", cex=0.8)

#DBSCAN
#downloaded using install.packages("dbscan")
library(dbscan)
db <- dbscan(cdataset, eps = 1.5, minPts = 5)
cld <- db$cluster+1
plot(cdataset, col = cld, pch = cld, xlab="X", ylab="Y")
title("DBScan")
legend(xlim[2]-6, ylim[2]-1, legend=unique(cld) , cex=0.8, col = unique(cld), pch = unique(cld), title="Clusters")
text(xlim[2]-5, ylim[1]+3, "SSE = \nPurity = ", cex=0.8)
