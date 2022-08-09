#!/usr/bin/env python3
import acts
from acts.examples import TGeoDetector
from pathlib import Path
import math

from acts.examples.reconstruction import (
    SeedfinderConfigArg,
    SeedFilterConfigArg,
    SpacePointGridConfigArg,
    SeedingAlgorithmConfigArg,
)

u = acts.UnitConstants


def buildITkGeometry(
    geo_dir: Path,
    material: bool = True,
    jsonconfig: bool = False,
    logLevel=acts.logging.WARNING,
):

    logger = acts.logging.getLogger("buildITkGeometry")

    matDeco = None
    if material:
        file = geo_dir / "itk-hgtd/material-maps-ITk-HGTD.json"
        logger.info("Adding material from %s", file.absolute())
        matDeco = acts.IMaterialDecorator.fromFile(
            file,
            level=acts.logging.Level(min(acts.logging.INFO.value, logLevel.value)),
        )

    tgeo_fileName = geo_dir / "itk-hgtd/ATLAS-ITk-HGTD.tgeo.root"

    if jsonconfig:
        jsonFile = geo_dir / "itk-hgtd/tgeo-atlas-itk-hgtd.json"
        logger.info("Create geometry from %s", jsonFile.absolute())
        return TGeoDetector.create(
            jsonFile=str(jsonFile),
            fileName=str(tgeo_fileName),
            surfaceLogLevel=logLevel,
            layerLogLevel=logLevel,
            volumeLogLevel=logLevel,
            mdecorator=matDeco,
        )

    Volume = TGeoDetector.Config.Volume
    LayerTriplet = TGeoDetector.Config.LayerTriplet
    equidistant = TGeoDetector.Config.BinningType.equidistant
    arbitrary = TGeoDetector.Config.BinningType.arbitrary

    return TGeoDetector.create(
        fileName=str(tgeo_fileName),
        mdecorator=matDeco,
        buildBeamPipe=True,
        unitScalor=1.0,  # explicit units
        beamPipeRadius=23.934 * u.mm,
        beamPipeHalflengthZ=3000.0 * u.mm,
        beamPipeLayerThickness=0.8 * u.mm,
        surfaceLogLevel=logLevel,
        layerLogLevel=logLevel,
        volumeLogLevel=logLevel,
        volumes=[
            Volume(
                name="InnerPixels",
                binToleranceR=(5 * u.mm, 5 * u.mm),
                binToleranceZ=(5 * u.mm, 5 * u.mm),
                binTolerancePhi=(0.025 * u.mm, 0.025 * u.mm),
                layers=LayerTriplet(True),
                subVolumeName=LayerTriplet("Pixel::Pixel"),
                sensitiveNames=LayerTriplet(["Pixel::siLog"]),
                sensitiveAxes=LayerTriplet("YZX"),
                rRange=LayerTriplet((0 * u.mm, 135 * u.mm)),
                zRange=LayerTriplet(
                    negative=(-3000 * u.mm, -250 * u.mm),
                    central=(-250 * u.mm, 250 * u.mm),
                    positive=(250 * u.mm, 3000 * u.mm),
                ),
                splitTolR=LayerTriplet(negative=-1.0, central=5 * u.mm, positive=-1.0),
                splitTolZ=LayerTriplet(
                    negative=10 * u.mm, central=-1.0, positive=10 * u.mm
                ),
                binning0=LayerTriplet(
                    negative=[(0, equidistant)],
                    central=[(6, equidistant), (10, equidistant)],
                    positive=[(0, equidistant)],
                ),
                binning1=LayerTriplet(
                    negative=[(0, equidistant)],
                    central=[(12, equidistant), (6, equidistant)],
                    positive=[(0, equidistant)],
                ),
                cylinderDiscSplit=False,
                cylinderNZSegments=0,
                cylinderNPhiSegments=0,
                discNRSegments=0,
                discNPhiSegments=0,
                itkModuleSplit=False,
                barrelMap={},
                discMap={},
            ),
            Volume(
                name="OuterPixels",
                binToleranceR=(5 * u.mm, 5 * u.mm),
                binToleranceZ=(5 * u.mm, 5 * u.mm),
                binTolerancePhi=(0.025 * u.mm, 0.025 * u.mm),
                layers=LayerTriplet(True),
                subVolumeName=LayerTriplet("Pixel::Pixel"),
                sensitiveNames=LayerTriplet(["Pixel::siLog"]),
                sensitiveAxes=LayerTriplet("YZX"),
                rRange=LayerTriplet((135 * u.mm, 350 * u.mm)),
                zRange=LayerTriplet(
                    negative=(-3000 * u.mm, -377 * u.mm),
                    central=(-377 * u.mm, 377 * u.mm),
                    positive=(377 * u.mm, 3000 * u.mm),
                ),
                splitTolR=LayerTriplet(
                    negative=15 * u.mm, central=5 * u.mm, positive=15 * u.mm
                ),
                splitTolZ=LayerTriplet(
                    negative=20 * u.mm, central=-1.0, positive=20 * u.mm
                ),
                binning0=LayerTriplet(
                    negative=[(0, equidistant)],
                    central=[(0, equidistant)],
                    positive=[(0, equidistant)],
                ),
                binning1=LayerTriplet(
                    negative=[(0, equidistant)],
                    central=[(0, equidistant)],
                    positive=[(0, equidistant)],
                ),
                cylinderDiscSplit=False,
                cylinderNZSegments=0,
                cylinderNPhiSegments=0,
                discNRSegments=0,
                discNPhiSegments=0,
                itkModuleSplit=False,
                barrelMap={},
                discMap={},
            ),
            Volume(
                name="Strips",
                binToleranceR=(5 * u.mm, 5 * u.mm),
                binToleranceZ=(5 * u.mm, 5 * u.mm),
                binTolerancePhi=(0.025 * u.mm, 0.025 * u.mm),
                layers=LayerTriplet(True),
                subVolumeName=LayerTriplet(
                    negative="*",
                    central="SCT::SCT_Barrel",
                    positive="*",
                ),
                sensitiveNames=LayerTriplet(
                    negative=["SCT::ECSensor*"],
                    central=["SCT::BRLSensor*"],
                    positive=["SCT::ECSensor*"],
                ),
                sensitiveAxes=LayerTriplet("XYZ"),
                rRange=LayerTriplet(
                    negative=(-1.0, 1050 * u.mm),
                    central=(380 * u.mm, 1050 * u.mm),
                    positive=(-1.0, 1050 * u.mm),
                ),
                zRange=LayerTriplet(
                    negative=(-3000 * u.mm, -1400 * u.mm),
                    central=(-1400 * u.mm, 1400 * u.mm),
                    positive=(1400 * u.mm, 3000 * u.mm),
                ),
                splitTolR=LayerTriplet(
                    negative=-1.0,
                    central=35 * u.mm,
                    positive=-1.0,
                ),
                splitTolZ=LayerTriplet(
                    negative=35 * u.mm, central=-1.0, positive=35 * u.mm
                ),
                binning0=LayerTriplet(
                    negative=[(-1, arbitrary)],
                    central=[(0, equidistant)],
                    positive=[(-1, arbitrary)],
                ),
                binning1=LayerTriplet(
                    negative=[(-1, arbitrary)],
                    central=[(28, equidistant)] * 4,
                    positive=[(-1, arbitrary)],
                ),
                cylinderDiscSplit=False,
                cylinderNZSegments=0,
                cylinderNPhiSegments=0,
                discNRSegments=0,
                discNPhiSegments=0,
                itkModuleSplit=True,
                barrelMap={"MS": 2, "SS": 4},
                discMap={
                    "EC0": [
                        [384.5, 403.481],
                        [403.481, 427.462],
                        [427.462, 456.442],
                        [456.442, 488.423],
                    ],
                    "EC1": [
                        [489.823, 507.916],
                        [507.916, 535.009],
                        [535.009, 559.101],
                        [559.101, 574.194],
                    ],
                    "EC2": [[575.594, 606.402], [606.402, 637.209]],
                    "EC3": [
                        [638.609, 670.832],
                        [670.832, 697.055],
                        [697.055, 723.278],
                        [723.278, 755.501],
                    ],
                    "EC4": [[756.901, 811.482], [811.482, 866.062]],
                    "EC5": [[867.462, 907.623], [907.623, 967.785]],
                },
            ),
            Volume(
                name="HGTD",
                binToleranceR=(15 * u.mm, 15 * u.mm),
                binToleranceZ=(5 * u.mm, 5 * u.mm),
                binTolerancePhi=(0.25 * u.mm, 0.25 * u.mm),
                layers=LayerTriplet(positive=True, central=False, negative=True),
                subVolumeName=LayerTriplet("HGTD::HGTD"),
                sensitiveNames=LayerTriplet(["HGTD::HGTDSiSensor*"]),
                sensitiveAxes=LayerTriplet("XYZ"),
                rRange=LayerTriplet(
                    negative=(0 * u.mm, 1050 * u.mm),
                    positive=(0 * u.mm, 1050 * u.mm),
                ),
                zRange=LayerTriplet(
                    negative=(-4000 * u.mm, -3000 * u.mm),
                    positive=(3000 * u.mm, 4000 * u.mm),
                ),
                splitTolR=LayerTriplet(-1.0),
                splitTolZ=LayerTriplet(negative=10 * u.mm, positive=10 * u.mm),
                binning0=LayerTriplet(
                    negative=[(0, equidistant)],
                    central=[(0, equidistant)],
                    positive=[(0, equidistant)],
                ),
                binning1=LayerTriplet(
                    negative=[(0, equidistant)],
                    central=[(0, equidistant)],
                    positive=[(0, equidistant)],
                ),
                cylinderDiscSplit=False,
                cylinderNZSegments=0,
                cylinderNPhiSegments=0,
                discNRSegments=0,
                discNPhiSegments=0,
                itkModuleSplit=False,
                barrelMap={},
                discMap={},
            ),
        ],
    )


def itkSeedingAlgConfig(inputSpacePointsType):

    # variables that do not change for pixel and strip SPs:
    zMax = 3000 * u.mm
    zMin = -3000 * u.mm
    beamPos = (0 * u.mm, 0 * u.mm)
    collisionRegionMin = -200 * u.mm
    collisionRegionMax = 200 * u.mm
    maxSeedsPerSpM = 4
    cotThetaMax = 27.2899
    sigmaScattering = 2
    radLengthPerSeed = 0.1
    minPt = 900 * u.MeV
    bFieldInZ = 2 * u.T
    deltaRMin = 20 * u.mm
    maxPtScattering = float("inf") * u.GeV
    zBinEdges = [
        -3000.0,
        -2500.0,
        -1400.0,
        -925.0,
        -450.0,
        -250.0,
        250.0,
        450.0,
        925.0,
        1400.0,
        2500.0,
        3000.0,
    ]  # zBinEdges enables non-equidistant binning in z, in case the binning is not defined the edges are evaluated automatically using equidistant binning
    rRangeMiddleSP = [
        [40.0, 90.0],
        [40.0, 200.0],
        [46.0, 200.0],
        [46.0, 200.0],
        [46.0, 250.0],
        [46.0, 250.0],
        [46.0, 250.0],
        [46.0, 200.0],
        [46.0, 200.0],
        [40.0, 200.0],
        [40.0, 90.0],
    ]  # if useVariableMiddleSPRange is set to false, the vector rRangeMiddleSP can be used to define a fixed r range for each z bin: {{rMin, rMax}, ...}. If useVariableMiddleSPRange is set to false and the vector is empty, the cuts won't be applied
    useVariableMiddleSPRange = True  # if useVariableMiddleSPRange is true, the values in rRangeMiddleSP will be calculated based on r values of the SPs and deltaRMiddleSPRange
    binSizeR = 1 * u.mm
    forceRadialSorting = True
    seedConfirmation = True
    centralSeedConfirmationRange = acts.SeedConfirmationRangeConfig(
        zMinSeedConf=-250 * u.mm,
        zMaxSeedConf=250 * u.mm,
        rMaxSeedConf=140 * u.mm,
        nTopForLargeR=1,
        nTopForSmallR=2,
    )  # contains parameters for seed confirmation
    forwardSeedConfirmationRange = acts.SeedConfirmationRangeConfig(
        zMinSeedConf=-3000 * u.mm,
        zMaxSeedConf=3000 * u.mm,
        rMaxSeedConf=140 * u.mm,
        nTopForLargeR=1,
        nTopForSmallR=2,
    )
    compatSeedWeight = 100
    curvatureSortingInFilter = True
    phiMin = 0
    phiMax = 2 * math.pi
    zBinEdges = [
        -3000.0,
        -2500.0,
        -1400.0,
        -925.0,
        -450.0,
        -250.0,
        250.0,
        450.0,
        925.0,
        1400.0,
        2500.0,
        3000.0,
    ]  # zBinEdges enables non-equidistant binning in z, in case the binning is not defined the edges are evaluated automatically using equidistant binning
    phiBinDeflectionCoverage = 3
    numPhiNeighbors = 1

    # variables that change for pixel and strip SPs:
    if inputSpacePointsType == "PixelSpacePoints":
        outputSeeds = "PixelSeeds"
        allowSeparateRMax = False
        rMaxGridConfig = 320 * u.mm
        rMaxSeedFinderConfig = rMaxGridConfig
        deltaRMinSP = 6 * u.mm
        deltaRMax = 280 * u.mm
        deltaRMaxTopSP = 280 * u.mm
        deltaRMaxBottomSP = 120 * u.mm
        interactionPointCut = True
        arithmeticAverageCotTheta = False
        deltaZMax = 600 * u.mm
        impactMax = 2 * u.mm
        zBinsCustomLooping = [
            1,
            2,
            3,
            4,
            11,
            10,
            9,
            8,
            6,
            5,
            7,
        ]  # enable custom z looping when searching for SPs, must contain numbers from 1 to the total number of bin in zBinEdges
        skipPreviousTopSP = True
        zBinNeighborsTop = [
            [0, 0],
            [-1, 0],
            [-1, 0],
            [-1, 0],
            [-1, 0],
            [-1, 1],
            [0, 1],
            [0, 1],
            [0, 1],
            [0, 1],
            [0, 0],
        ]  # allows to specify the number of neighbors desired for each bin, [-1,1] means one neighbor on the left and one on the right, if the vector is empty the algorithm returns the 8 surrounding bins
        zBinNeighborsBottom = [
            [0, 1],
            [0, 1],
            [0, 1],
            [0, 1],
            [0, 1],
            [0, 0],
            [-1, 0],
            [-1, 0],
            [-1, 0],
            [-1, 0],
            [-1, 0],
        ]
        deltaRMiddleMinSPRange = 10 * u.mm
        deltaRMiddleMaxSPRange = 10 * u.mm
        seedConfirmationFilter = True
        impactWeightFactor = 100
        compatSeedLimit = 3
        numSeedIncrement = 10**100  # inf
        seedWeightIncrement = 0
        useDetailedDoubleMeasurementInfo = False
        maxSeedsPerSpMConf = 5
        maxQualitySeedsPerSpMConf = 5
        useDeltaRorTopRadius = True
    else:
        outputSeeds = "StripSeeds"
        allowSeparateRMax = True
        rMaxGridConfig = 1000.0 * u.mm
        rMaxSeedFinderConfig = 1200.0 * u.mm
        deltaRMinSP = 20 * u.mm
        deltaRMax = 600 * u.mm
        deltaRMaxTopSP = 300 * u.mm
        deltaRMaxBottomSP = deltaRMaxTopSP
        interactionPointCut = False
        arithmeticAverageCotTheta = True
        deltaZMax = 900 * u.mm
        impactMax = 20 * u.mm
        zBinsCustomLooping = [6, 7, 5, 8, 4, 9, 3, 10, 2, 11, 1]
        skipPreviousTopSP = False
        zBinNeighborsTop = [
            [0, 0],
            [-1, 0],
            [-1, 0],
            [-1, 0],
            [-1, 0],
            [-1, 1],
            [0, 1],
            [0, 1],
            [0, 1],
            [0, 1],
            [0, 0],
        ]
        zBinNeighborsBottom = [
            [0, 1],
            [0, 1],
            [0, 1],
            [0, 2],
            [0, 1],
            [0, 0],
            [-1, 0],
            [-2, 0],
            [-1, 0],
            [-1, 0],
            [-1, 0],
        ]
        deltaRMiddleMinSPRange = 30 * u.mm
        deltaRMiddleMaxSPRange = 150 * u.mm
        seedConfirmationFilter = False
        impactWeightFactor = 1
        compatSeedLimit = 4
        numSeedIncrement = 1
        seedWeightIncrement = 10100
        useDetailedDoubleMeasurementInfo = True
        maxSeedsPerSpMConf = 1000000000
        maxQualitySeedsPerSpMConf = 1000000000
        useDeltaRorTopRadius = False

    # fill namedtuples
    seedfinderConfigArg = SeedfinderConfigArg(
        maxSeedsPerSpM=maxSeedsPerSpM,
        cotThetaMax=cotThetaMax,
        sigmaScattering=sigmaScattering,
        radLengthPerSeed=radLengthPerSeed,
        minPt=minPt,
        bFieldInZ=bFieldInZ,
        impactMax=impactMax,
        interactionPointCut=interactionPointCut,
        arithmeticAverageCotTheta=arithmeticAverageCotTheta,
        deltaZMax=deltaZMax,
        maxPtScattering=maxPtScattering,
        zBinEdges=zBinEdges,
        skipPreviousTopSP=skipPreviousTopSP,
        zBinsCustomLooping=zBinsCustomLooping,
        rRangeMiddleSP=rRangeMiddleSP,
        useVariableMiddleSPRange=useVariableMiddleSPRange,
        binSizeR=binSizeR,
        forceRadialSorting=forceRadialSorting,
        seedConfirmation=seedConfirmation,
        centralSeedConfirmationRange=centralSeedConfirmationRange,
        forwardSeedConfirmationRange=forwardSeedConfirmationRange,
        deltaR=(deltaRMin, deltaRMax),
        deltaRBottomSP=(deltaRMinSP, deltaRMaxBottomSP),
        deltaRTopSP=(deltaRMinSP, deltaRMaxTopSP),
        deltaRMiddleSPRange=(deltaRMiddleMinSPRange, deltaRMiddleMaxSPRange),
        collisionRegion=(collisionRegionMin, collisionRegionMax),
        r=(None, rMaxSeedFinderConfig),
        z=(zMin, zMax),
        beamPos=beamPos,
    )
    seedFilterConfigArg = SeedFilterConfigArg(
        impactWeightFactor=impactWeightFactor,
        compatSeedWeight=compatSeedWeight,
        compatSeedLimit=compatSeedLimit,
        numSeedIncrement=numSeedIncrement,
        seedWeightIncrement=seedWeightIncrement,
        seedConfirmation=seedConfirmation,
        curvatureSortingInFilter=curvatureSortingInFilter,
        maxSeedsPerSpMConf=maxSeedsPerSpMConf,
        maxQualitySeedsPerSpMConf=maxQualitySeedsPerSpMConf,
        useDeltaRorTopRadius=useDeltaRorTopRadius,
    )
    spacePointGridConfigArg = SpacePointGridConfigArg(
        rMax=rMaxGridConfig,
        zBinEdges=zBinEdges,
        phiBinDeflectionCoverage=phiBinDeflectionCoverage,
        phi=(phiMin, phiMax),
        impactMax=impactMax,
    )
    seedingAlgorithmConfigArg = SeedingAlgorithmConfigArg(
        allowSeparateRMax=allowSeparateRMax,
        zBinNeighborsTop=zBinNeighborsTop,
        zBinNeighborsBottom=zBinNeighborsBottom,
        numPhiNeighbors=numPhiNeighbors,
    )

    return (
        seedfinderConfigArg,
        seedFilterConfigArg,
        spacePointGridConfigArg,
        seedingAlgorithmConfigArg,
    )