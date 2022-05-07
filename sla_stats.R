library(reshape2)
library(ggplot2)
library(scales)
library(ggpubr)
library(patchwork)
library(xtable)
library(gridExtra)

SA <- read.table("data/SA.data",
                 header = T,
                 row.names = 1)
MA <- read.table("data/MA.data", header = T, row.names = 1)
best <-
  read.table("mkp_instances/best_known_values.txt",
             header = T,
             row.names = 1)

relative_percentage_deviation <- function(value) {
  relative <- 100 * (best$best - value) / best$best
  
  return (relative)
  
}

SA_rel <- relative_percentage_deviation(SA)
MA_rel <- relative_percentage_deviation(MA)

print(mean(SA_rel[1:30, ]))
print(mean(SA_rel[31:60, ]))
print(mean(MA_rel[1:30, ]))
print(mean(MA_rel[31:60, ]))

pdf('../2/correlation.pdf', width = 7, height = 7)
ggplot(NULL) +
  geom_point(aes(x = SA_rel[1:30,], y = MA_rel[1:30,], col = "100")) +
  geom_point(aes(x = SA_rel[31:60,], y = MA_rel[31:60,], col = "250")) +
  theme_linedraw() + theme(legend.position = "bottom") +
  ylim(-0.06, 0.06) +
  geom_smooth(
    method = 'lm',
    formula = y ~ x,
    aes(x = SA_rel[1:30,], y = MA_rel[1:30,], col = "100"),
    fullrange = TRUE,
    alpha = 0.1
  ) +
  geom_smooth(
    method = 'lm',
    formula = y ~ x,
    aes(x = SA_rel[31:60,], y = MA_rel[31:60,], col = "250"),
    fullrange = TRUE,
    alpha = 0.1
  ) +
  labs(x = "Relative percentage deviation - Simulated annealing", y = "Relative percentage deviation - Memetic algorithm", col =
         "Instance size")
dev.off()

print(wilcox.test(SA_rel[1:30,], MA_rel[1:30,], paired = TRUE))
print(wilcox.test(SA_rel[31:60,], MA_rel[31:60,], paired = TRUE))

pdf('../2/boxplot.pdf', width = 7, height = 7)
ggplot(melt(data.frame(SA_rel, MA_rel)), aes(variable, value)) + geom_boxplot() + theme_linedraw() +
  scale_x_discrete(labels = c("Simulated annealing", "Memetic lgorithm")) +
  labs(x = "Algorithm", y = "Relative percentage deviation")
dev.off()

five_best <- best[32:36, ]

five_relative_percentage_deviation <- function(value) {
  relative <- t(100 * (five_best - t(value)) / five_best)
  
  return (relative)
  
}

SA1 <- read.table("data/rtd/SA_1.data",
                  header = T)
SA10 <- read.table("data/rtd/SA_10.data",
                   header = T)
SA100 <- read.table("data/rtd/SA_100.data",
                    header = T)

SA1_rel <- five_relative_percentage_deviation(SA1)
SA10_rel <- five_relative_percentage_deviation(SA10)
SA100_rel <- five_relative_percentage_deviation(SA100)

MA1 <- read.table("data/rtd/MA_1.data",
                  header = T)
MA10 <- read.table("data/rtd/MA_10.data",
                   header = T)
MA100 <- read.table("data/rtd/MA_100.data",
                    header = T)

MA1_rel <- five_relative_percentage_deviation(MA1)
MA10_rel <- five_relative_percentage_deviation(MA10)
MA100_rel <- five_relative_percentage_deviation(MA100)

l <- c()
for (i in 1:5) {
  l[[2 * i - 1]] <- local({
    i <- i
    p1 <- ggplot(NULL) +
      stat_ecdf(aes(SA100_rel[, i], col = "max-time x 0.01"), geom = "step") +
      stat_ecdf(aes(SA10_rel[, i], col = "max-time x 0.1"), geom = "step") +
      stat_ecdf(aes(SA1_rel[, i], col = "max-time x 1"), geom = "step") +
      theme_linedraw() +
      labs(x = NULL, y = NULL, color = "Run-time") +
      xlim(0, 8)
    print(p1)
  })
  l[[2 * i]] <- local({
    i <- i
    p1 <- ggplot(NULL) +
      stat_ecdf(aes(MA100_rel[, i], col = "max-time x 0.01"), geom = "step") +
      stat_ecdf(aes(MA10_rel[, i], col = "max-time x 0.1"), geom = "step") +
      stat_ecdf(aes(MA1_rel[, i], col = "max-time x 1"), geom = "step") +
      theme_linedraw() +
      labs(x = NULL, y = NULL, color = "Run -time") +
      xlim(-0.07, 0.5)
    print(p1)
  })
}

pdf('../2/sqd.pdf',
    width = 7,
    height = 11,
    onefile = FALSE)
ggarrange(
  plotlist = l,
  common.legend = TRUE,
  legend = "bottom",
  ncol = 2,
  nrow = 5
)
dev.off()

print(xtable(data.frame(SA_rel, MA_rel), type = "latex"), digits = c(4, 4, 4))

cor.test(SA_rel[1:30,], MA_rel[1:30,], method="spearman")
cor.test(SA_rel[31:60,], MA_rel[31:60,], method="spearman")

wilcox.test(relative_percentage_deviation(SA[,]), relative_percentage_deviation(SA_t[,]), paired=TRUE)
